/* Copyright 2014 CyberTech Labs Ltd.
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

#include <QtCore/qglobal.h>

#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
	#include <QtGui/QWidget>
#else
	#include <QtWidgets/QWidget>
#endif

#include <QtCore/QEventLoop>

namespace trikGui {

class TrikGuiDialog : public QWidget
{
	Q_OBJECT

public:
	explicit TrikGuiDialog(QWidget *parent = 0);

	virtual ~TrikGuiDialog() {}

	enum Result {
		normalExit
		, goHomeExit
	};

	virtual int exec();

protected:
	virtual void keyPressEvent(QKeyEvent *event);

	virtual void exit();

	virtual void goHome();

private:
	QEventLoop mEventLoop;
};

}
