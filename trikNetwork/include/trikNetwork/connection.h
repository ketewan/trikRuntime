/* Copyright 2014 - 2015 CyberTech Labs Ltd.
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
#include <QtCore/QScopedPointer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostAddress>

#include "declSpec.h"

namespace trikNetwork {

/// Connection protocol variants.
enum class Protocol
{
	/// Message is in form "<data length in bytes>:<data>".
	messageLength

	/// Message is in form "<data>\n".
	, endOfLineSeparator
};

/// Abstract class that serves one client of TrikServer. Meant to work in separate thread. Creates its own socket and
/// handles all incoming messages.
class TRIKNETWORK_EXPORT Connection : public QObject
{
	Q_OBJECT

public:
	/// Constructor.
	/// @param connectionProtocol - protocol used by this connection.
	explicit Connection(Protocol connectionProtocol);

	/// Returns true if socket is opened or false otherwise.
	bool isConnected() const;

	/// Returns peer address of a connection, if it is open, or empty QHostAddress if connection is not established yet.
	QHostAddress peerAddress() const;

	/// Returns peer port of a connection, if it is open, or -1 if connection is not established yet.
	int peerPort() const;

	/// Creates socket and initializes incoming connection, shall be called when Connection is already in its own
	/// thread.
	/// @param socketDescriptor - native socket descriptor.
	Q_INVOKABLE void init(int socketDescriptor);

	/// Sends given byte array to peer.
	Q_INVOKABLE void send(const QByteArray &data);

signals:
	/// Emitted after connection becomes closed.
	void disconnected();

protected:
	/// Creates socket and initializes outgoing connection, shall be called when Connection is already in its own
	/// thread.
	/// @param ip - target ip address.
	/// @param port - target port.
	void init(const QHostAddress &ip, int port);

private slots:
	/// New data is ready on a socket.
	void onReadyRead();

	void onConnect();

	/// Socket is closed for some reason.
	void onDisconnect();

	/// Socket is failed to connect or some other error occured.
	void onError(QAbstractSocket::SocketError error);

	void onTimeout();

private:
	/// Processes received data. Shall be implemented in concrete connection classes.
	virtual void processData(const QByteArray &data) = 0;

	/// Handles incoming data: sending version or processing received data.
	void handleIncomingData(const QByteArray &data);

	void connectSlots();

	void processBuffer();

	/// Socket for this connection.
	QScopedPointer<QTcpSocket> mSocket;

	/// Buffer to accumulate parts of a message.
	QByteArray mBuffer;

	/// Declared size of a current message.
	int mExpectedBytes = 0;

	Protocol mProtocol;

	QTimer mKeepAliveTimer;
};

}
