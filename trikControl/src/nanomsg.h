/* Copyright 2015 CyberTech Labs Ltd.
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

#include "nanomsgInterface.h"
#include "deviceState.h"

namespace trikKernel {
class Configurer;
}

namespace trikHal {
class HardwareAbstractionInterface;
class NanomsgInterface;
}

namespace trikControl {

class Nanomsg: public NanomsgInterface
{
	Q_OBJECT

public:
	/// @param virtualPort - port in system-config.xml
	/// @param configurer - configurer object containing preparsed XML files with parameters.
	/// @param hardwareAbstraction - interface to underlying hardware or operating system capabilities of a robot.
	Nanomsg(const QString &virtualPort, const trikKernel::Configurer &configurer
			, const trikHal::HardwareAbstractionInterface &hardwareAbstraction);

	/// @param fileName - ipc:///tmp/[node, server]_fileName.ipc
	/// @param hardwareAbstraction - interface to underlying hardware or operating system capabilities of a robot.
	Nanomsg(const QString &fileName, const trikHal::HardwareAbstractionInterface &hardwareAbstraction);

	~Nanomsg() override;

	Status status() const override;

public slots:
	/// Reads all available data.
	QString read() override;

	/// Returns true if there is new data.
	bool hasData() const override;

	///Server has to be specified by file address.
	/// Sends request to server.
	bool sendRequest(const QString &request) override;

	/// Subscribes for data published by server.
	bool subscribe() override;

private slots:
	void onNewData(const QString &data);
	void onNewError();

private:
	QScopedPointer<trikHal::NanomsgInterface> mNanomsg;

	/// Last line that was read.
	QString mCurrent;

	/// State of a file as a device.
	DeviceState mState;
};

}
