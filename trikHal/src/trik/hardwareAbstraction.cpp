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

#include "hardwareAbstraction.h"

#include "mspI2c.h"
#include "mspUsb.h"
#include "systemConsole.h"
#include "eventFile.h"
#include "inputDeviceFile.h"
#include "outputDeviceFile.h"
#include "fifo.h"
#include "nanomsg.h"

using namespace trikHal;

HardwareAbstraction::HardwareAbstraction()
	: mI2c(new MspI2c())
	, mUsb(new MspUsb())
	, mSystemConsole(new SystemConsole())
{
}

HardwareAbstraction::~HardwareAbstraction()
{
}

MspI2cInterface &HardwareAbstraction::mspI2c()
{
	return *mI2c.data();
}

MspUsbInterface &HardwareAbstraction::mspUsb()
{
	return *mUsb.data();
}

SystemConsoleInterface &HardwareAbstraction::systemConsole()
{
	return *mSystemConsole.data();
}

EventFileInterface *HardwareAbstraction::createEventFile(const QString &fileName, QThread &thread) const
{
	return new EventFile(fileName, thread);
}

FifoInterface *HardwareAbstraction::createFifo(const QString &fileName) const
{
	return new Fifo(fileName);
}

NanomsgInterface *HardwareAbstraction::createNanomsg(const QString &fileName) const
{
	return new Nanomsg(fileName);
}

InputDeviceFileInterface *HardwareAbstraction::createInputDeviceFile(const QString &fileName) const
{
	return new InputDeviceFile(fileName);
}

OutputDeviceFileInterface *HardwareAbstraction::createOutputDeviceFile(const QString &fileName) const
{
	return new OutputDeviceFile(fileName);
}
