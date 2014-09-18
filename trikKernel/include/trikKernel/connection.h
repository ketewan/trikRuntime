/* Copyright 2014 CyberTech Labs Ltd.
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

#pragma once

#include <QtCore/QObject>
#include <QtCore/QScopedPointer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

namespace trikKernel {

/// Abstract class that serves one client of TrikServer. Meant to work in separate thread. Creates its own socket and
/// handles all incoming messages.
class Connection : public QObject {
	Q_OBJECT

public:
	QHostAddress peerAddress() const;
	int peerPort() const;

public slots:
	/// Creates socket and initializes incoming connection, shall be called when Connection is already in its own
	/// thread.
	/// @param socketDescriptor - native socket descriptor.
	void onInit(int socketDescriptor);

	void onSend(QByteArray const &data);

protected:
	/// Creates socket and initializes outgoing connection, shall be called when Connection is already in its own
	/// thread.
	/// @param ip - target ip address.
	/// @param port - target port.
	void onInit(QHostAddress const &ip, int port);


private slots:
	/// New data is ready on a socket.
	void onReadyRead();

	/// Socket is closed for some reason.
	void disconnected();

	/// Socket is failed to connect or some other error occured.
	void errored(QAbstractSocket::SocketError error);

private:
	/// Processes received data. Shall be implemented in concrete connection classes.
	virtual void processData(QString const &data) = 0;

	void connectSlots();

	/// Socket for this connection.
	QScopedPointer<QTcpSocket> mSocket;
};

}
