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

#include "mspI2c.h"

#include <QsLog.h>

using namespace trikHal;

void MspI2c::send(const QByteArray &data)
{
	QLOG_INFO() << "Sending thru MSP I2C stub" << data;
}

int MspI2c::read(const QByteArray &data)
{
	QLOG_INFO() << "Reading from MSP I2C stub" << data;
	return 0;
}

bool MspI2c::connect(const QString &devicePath, int deviceId)
{
	QLOG_INFO() << "Connecting to MSP I2C stub, devicePath:" << devicePath << "deviceId" << deviceId;
	return true;
}

void MspI2c::disconnect()
{
	QLOG_INFO() << "Disconnecting from MSP I2C stub";
}

MspI2c::~MspI2c()
{
}
