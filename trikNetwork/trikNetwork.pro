# Copyright 2015 CyberTech Labs Ltd.
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

TEMPLATE = lib

include(../global.pri)

PUBLIC_HEADERS += \
	$$PWD/include/trikNetwork/connection.h \
	$$PWD/include/trikNetwork/declSpec.h \
	$$PWD/include/trikNetwork/mailboxFactory.h \
	$$PWD/include/trikNetwork/mailboxInterface.h \
	$$PWD/include/trikNetwork/trikServer.h \
	$$PWD/include/trikNetwork/trikCoapServer.h \

HEADERS += \
	$$PWD/src/mailbox.h \
	$$PWD/src/mailboxConnection.h \
	$$PWD/src/mailboxServer.h \

SOURCES += \
	$$PWD/src/connection.cpp \
	$$PWD/src/mailbox.cpp \
	$$PWD/src/mailboxConnection.cpp \
	$$PWD/src/mailboxFactory.cpp \
	$$PWD/src/mailboxServer.cpp \
	$$PWD/src/trikServer.cpp \
	src/trikCoapServer.cpp

#QMAKE_RPATHLINKDIR = /home/arsonist/coap/installed_emb_2/lib

#LIBS += -L/home/arsonist/coap/installed_emb_2/lib -lcoap-2


INCLUDEPATH += $$PWD/include \
	$$PWD/../qslog \
	$$PWD/../trikKernel/include \
	/home/arsonist/coap/installed_emb_2/include

QT += network

DEFINES += TRIKNETWORK_LIBRARY

implementationIncludes(trikControl coap-2)
system(ln -svt $$GLOBAL_DESTDIR /home/arsonist/coap/installed_emb_2/lib/libcoap-2.so* )

links(qslog trikKernel trikControl coap-2)

installs()
