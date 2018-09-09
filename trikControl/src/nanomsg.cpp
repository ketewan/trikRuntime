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

#include "src/nanomsg.h"

#include <QtCore/QEventLoop>

#include <trikKernel/configurer.h>
#include <trikHal/hardwareAbstractionInterface.h>

#include <QsLog.h>

#include "src/configurerHelper.h"

using namespace trikControl;

Nanomsg::Nanomsg(const QString &virtualPort, const trikKernel::Configurer &configurer
                , const trikHal::HardwareAbstractionInterface &hardwareAbstraction)
        : Nanomsg(configurer.attributeByPort(virtualPort, "file"), hardwareAbstraction)
{
}

Nanomsg::Nanomsg(const QString &fileName, const trikHal::HardwareAbstractionInterface &hardwareAbstraction)
        : mNanomsg(hardwareAbstraction.createNanomsg(fileName))
        , mState("Nanomsg client on '" + fileName + "'")
{
    mState.start();

    connect(mNanomsg.data(), SIGNAL(newData(QString)), this, SLOT(onNewData(QString)));
    connect(mNanomsg.data(), SIGNAL(newError()), this, SLOT(onNewError()));

    if (mNanomsg->connect()) {
        mState.ready();
    } else {
        mState.fail();
    }
}

Nanomsg::~Nanomsg()
{
    if (mState.isReady()) {
        mNanomsg->close();
    }
}

DeviceInterface::Status Nanomsg::status() const
{
    return mState.status();
}

QString Nanomsg::read()
{
    //???
        while (mCurrent.isEmpty()) {
        QEventLoop eventLoop;
        connect(this, SIGNAL(newData(QString)), &eventLoop, SLOT(quit()));
        eventLoop.exec();
    }
    //???

    const QString result = mCurrent;
    mCurrent = "";
    return result;
}

bool Nanomsg::hasData() const
{
    return mCurrent != "";
}

bool Nanomsg::subscribe()  {
    return mNanomsg->subscribe();
}

bool Nanomsg::sendRequest(const QString &request) {
    return mNanomsg->sendRequest(request);
}

void Nanomsg::onNewData(const QString &data)
{
    QString buffer = data;
    mCurrent.swap(buffer);
    emit newData(data);
}

void Nanomsg::onNewError()
{
    mState.fail();
}
