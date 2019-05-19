#pragma once

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <QObject>
#include <QTimer>

#include "/home/arsonist/coap/installed_emb/usr/local/include/coap2/coap.h"
#include "declSpec.h"
#include <trikControl/brickInterface.h>

namespace trikControl {
class BrickInterface;
}

namespace trikNetwork {

class TRIKNETWORK_EXPORT TrikCoapServer : public QObject{
	Q_OBJECT

public:
	//TrikCoapServer();
	static TrikCoapServer* instance;
	TrikCoapServer(trikControl::BrickInterface &brick);
	static int resolve_address(const char *host, const char *service, coap_address_t *dst);

	static void hnd_get_hello(coap_context_t *ctx,
				 coap_resource_t *resource,
				 coap_session_t *session,
				 coap_pdu_t *request,
				 coap_binary_t *token,
				 coap_string_t *query,
				 coap_pdu_t *response);

	static void hnd_get_distance(coap_context_t *ctx,
				 coap_resource_t *resource,
				 coap_session_t *session,
				 coap_pdu_t *request,
				 coap_binary_t *token,
				 coap_string_t *query,
				 coap_pdu_t *response);
	Q_INVOKABLE void start(void);

	QVector<coap_resource_t *> observable_resources;
	QVector<QPair<QString, trikControl::SensorInterface *>> sensors;
	QVector<QPair<QString, trikControl::MotorInterface *>> motors;
	trikControl::BrickInterface &mBrick;

private slots:
	void coapRunOnce();

private:
	void init_sensors();
	void init_motors();
	void get_distance();
	void init_resources();
	void sensor_handler();

	coap_context_t *ctx = nullptr;
	coap_address_t dst;
	coap_resource_t *resource = nullptr;
	coap_endpoint_t *endpoint = nullptr;
	coap_resource_t *distance = nullptr;
	QTimer *timer;
};
}
