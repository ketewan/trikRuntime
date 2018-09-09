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

#include "deviceInterface.h"

#include "declSpec.h"

namespace trikControl {

/// Interface that represents nanomsg client
class TRIKCONTROL_EXPORT NanomsgInterface : public QObject, public DeviceInterface
{
    Q_OBJECT

public slots:
    /// Returns true if there is new data.
    virtual bool hasData() const = 0;

    /// Reads all available data.
    virtual QString read() = 0;

    ///Server has to be specified by file address.
    /// Sends request to server.
    virtual bool sendRequest(const QString &request) = 0;

    /// Subscribe for data published by server.
    virtual bool subscribe() = 0;

signals:
    /// Emitted when new string has arrived from server.
    void newData(const QString &data);
};

}
