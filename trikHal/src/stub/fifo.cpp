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

#include "fifo.h"

#include <QsLog.h>

using namespace trikHal;

Fifo::Fifo(const QString &fileName)
	: mFileName(fileName)
{
}

bool Fifo::open()
{
	QLOG_INFO() << "Opening stub fifo" << mFileName;
	return true;
}

bool Fifo::close()
{
	QLOG_INFO() << "Closing stub fifo" << mFileName;
	return true;
}

QString Fifo::fileName()
{
	return mFileName;
}

Fifo::~Fifo()
{
}

void Fifo::readFile()
{
}
