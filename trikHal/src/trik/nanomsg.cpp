/* Copyright 2015 Yurii Litvinov and CyberTech Labs Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License. */

#include "nanomsg.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <iostream>
#include <nanomsg/nn.h>
#include <nanomsg/pubsub.h>
#include <nanomsg/reqrep.h>

#include <QtCore/QSocketNotifier>
#include <QtCore/QStringList>

#include <QsLog.h>

using namespace trikHal;

Nanomsg::Nanomsg(const QString &fileName)
	: mFileName(fileName)
	, mSubscriberFD(-1)
{
}

Nanomsg::~Nanomsg()
{
	close();
}

bool Nanomsg::connect()
{
	if ((mRequester = nn_socket(AF_SP, NN_REQ)) < 0) {
		QLOG_ERROR() << "connect nn_socket error";
		return false;
	}

	QString replier = "ipc:///tmp/node_" + fileName() + ".ipc";

	QLOG_ERROR() << replier.toLatin1().data();
	if ((nn_connect(mRequester, replier.toLatin1().data())) < 0) {
		QLOG_ERROR() << "connect nn_connect error";
		return false;
	}

	QLOG_INFO() << "Connected to sensor's replier";

	//Prepare to subscribe later if needed
	if ((mSubscriber = nn_socket(AF_SP, NN_SUB)) < 0) {
		QLOG_ERROR() << "subscribe nn_socket error";
		return false;
	}

	size_t sz = sizeof(mSubscriberFD);
	nn_getsockopt(mSubscriber, NN_SOL_SOCKET, NN_RCVFD, &mSubscriberFD, &sz);
	mSocketNotifier.reset(new QSocketNotifier(mSubscriberFD, QSocketNotifier::Read));
	QObject::connect(mSocketNotifier.data(), SIGNAL(activated(int)), this, SLOT(readData()));
	mSocketNotifier->setEnabled(true);

	return true;
}

bool Nanomsg::close() {
	if (nn_close(mSubscriber) < 0)
		return false;
	if (nn_close(mRequester) < 0)
		return false;
	return true;
}

bool Nanomsg::subscribe() {

	if (nn_setsockopt(mSubscriber, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) < 0) {
		QLOG_ERROR() << "subscribe nn_setsockopt error";
		return false;
	}

	QString publisher = "ipc:///tmp/server_" + fileName() + ".ipc";

	if (nn_connect(mSubscriber, publisher.toLatin1().data()) < 0) {
		QLOG_ERROR() << "subscribe nn_connect error";
		return false;
	}

	return true;
}

bool Nanomsg::sendRequest(const QString &request) {
	auto len = request.length() + 1;
	if (nn_send(mRequester, request.toLatin1().data(), len, 0) < 0) {
		QLOG_ERROR() << "sendRequest nn_send error";
		return false;
	}

	char *buf = NULL;

	if (nn_recv(mRequester, &buf, NN_MSG, 1) < 0) {
		QLOG_ERROR() << "sendRequest nn_recv error";
		return false;
	}

	return true;
}

void Nanomsg::readData()
{
	mSocketNotifier->setEnabled(false);

	char *buf = NULL;
	int bytes = -1;

	// recieve ALL available messages
	while ((bytes = nn_recv(mSubscriber, &buf, NN_MSG, 1)) > 0) {
		//QLOG_ERROR() << "readData nn_recv error";
		//emit newError();
		//return;
		QString mBuffer(QByteArray(buf, bytes));
		QStringList lines = mBuffer.split('\n', QString::SkipEmptyParts);

		for (const QString &line : lines) {
			emit newData(line);
		}

		nn_freemsg(buf);
	}

	mSocketNotifier->setEnabled(true);
}

QString Nanomsg::fileName()
{
	return mFileName;
}
