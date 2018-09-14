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

#pragma once

#include <QtCore/QScopedPointer>

#include "nanomsgInterface.h"

class QSocketNotifier;

namespace trikHal {

class Nanomsg : public NanomsgInterface
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param fileName - name of a server/replier file.
	explicit Nanomsg(const QString &fileName);

	~Nanomsg() override;

	bool connect() override;
	bool close() override;
	bool subscribe() override;
	bool sendRequest(const QString &request) override;

	QString fileName() override;

private slots:
	/// Called when there is new data.
	void readData();

private:
	/// Name of a server/replier file.
	const QString mFileName;

	/// Socket files.
	int mSubscriber;
	int mRequester;

	/// Only for notifies, not for read/write
	int mSubscriberFD;

	/// Notifier for subscriber fd that emits a signal when something is changed in it.
	QScopedPointer<QSocketNotifier> mSocketNotifier;
};

}
