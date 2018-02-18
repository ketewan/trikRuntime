# Copyright 2015 Yurii Litvinov and CyberTech Labs Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

include(../global.pri)

INCLUDEPATH += $$PWD/src

PUBLIC_HEADERS += \
	$$PWD/include/trikHal/declSpec.h \
	$$PWD/include/trikHal/hardwareAbstractionInterface.h \
	$$PWD/include/trikHal/hardwareAbstractionFactory.h \
	$$PWD/include/trikHal/fifoInterface.h \
	$$PWD/include/trikHal/eventFileInterface.h \
	$$PWD/include/trikHal/inputDeviceFileInterface.h \
	$$PWD/include/trikHal/mspI2cInterface.h \
	$$PWD/include/trikHal/mspUsbInterface.h \
	$$PWD/include/trikHal/outputDeviceFileInterface.h \
	$$PWD/include/trikHal/systemConsoleInterface.h \

HEADERS += \
	$$PWD/src/hardwareAbstraction.h \
	$$PWD/src/mspI2c.h \
	$$PWD/src/mspUsb.h \
	$$PWD/src/systemConsole.h \
	$$PWD/src/eventFile.h \
	$$PWD/src/inputDeviceFile.h \
	$$PWD/src/outputDeviceFile.h \
	$$PWD/src/fifo.h \

SOURCES += \
	$$PWD/src/hardwareAbstractionFactory.cpp \

equals(ARCHITECTURE, arm) {
	HEADERS += \
		$$PWD/src/trik/usbMsp/usbMSP430Interface.h \
		$$PWD/src/trik/usbMsp/usbMSP430Defines.h \

	SOURCES += \
		$$PWD/src/trik/hardwareAbstraction.cpp \
		$$PWD/src/trik/mspI2c.cpp \
		$$PWD/src/trik/mspUsb.cpp \
		$$PWD/src/trik/systemConsole.cpp \
		$$PWD/src/trik/eventFile.cpp \
		$$PWD/src/trik/inputDeviceFile.cpp \
		$$PWD/src/trik/outputDeviceFile.cpp \
		$$PWD/src/trik/fifo.cpp \
		$$PWD/src/trik/usbMsp/usbMSP430Interface.cpp \
}

else:unix {
	SOURCES += \
		$$PWD/src/stub/hardwareAbstraction.cpp \
		$$PWD/src/stub/mspI2c.cpp \
		$$PWD/src/stub/mspUsb.cpp \
                $$PWD/src/trik/systemConsole.cpp \
		$$PWD/src/stub/eventFile.cpp \
		$$PWD/src/stub/inputDeviceFile.cpp \
		$$PWD/src/stub/outputDeviceFile.cpp \
		$$PWD/src/trik/fifo.cpp \
}

else {
	SOURCES += \
		$$PWD/src/stub/hardwareAbstraction.cpp \
		$$PWD/src/stub/mspI2c.cpp \
		$$PWD/src/stub/mspUsb.cpp \
		$$PWD/src/stub/systemConsole.cpp \
		$$PWD/src/stub/eventFile.cpp \
		$$PWD/src/stub/inputDeviceFile.cpp \
		$$PWD/src/stub/outputDeviceFile.cpp \
		$$PWD/src/stub/fifo.cpp \
}

TEMPLATE = lib

DEFINES += TRIKHAL_LIBRARY

links(trikKernel)
implementationIncludes(trikKernel)

if (equals(QT_MAJOR_VERSION, 5)) {
	QT += widgets
}

installs()
