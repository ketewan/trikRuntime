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

#include <QtCore/QString>
#include <QtCore/QFile>

#include "outputDeviceFileInterface.h"

namespace trikHal {

class OutputDeviceFile : public OutputDeviceFileInterface
{
public:
	/// Constructor.
	/// @param fileName - name of a device file .
    OutputDeviceFile(const QString &fileName);

	bool open() override;
	void close() override;
	void write(const QString &data) override;
	QString fileName() const override;

private:
	/// Underlying file.
	QFile mFile;
};

}
