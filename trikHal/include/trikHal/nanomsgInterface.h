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

#include <QtCore/QString>
#include <QtCore/QObject>

namespace trikHal {

class NanomsgInterface : public QObject
{
    Q_OBJECT

public:
    /// Connects to replier.
    /// @returns true, if connected successfully.
    virtual bool connect() = 0;

    virtual bool close() = 0;

    virtual bool subscribe() = 0;

    virtual bool sendRequest(const QString &request) = 0;

    /// Returns file name of a server/replier.
    /// ipc://tmp/[server, node]_filename.ipc
    virtual QString fileName() = 0;

signals:
    /// Emitted when there is new data.
    void newData(const QString &data);

    /// Emitted when something is wrong
    void newError();
};

}
