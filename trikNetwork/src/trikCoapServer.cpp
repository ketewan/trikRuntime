#include <QtCore/QDebug>
#include <functional>
#include <QsLog.h>
#include "trikCoapServer.h"

using namespace trikNetwork;
TrikCoapServer* TrikCoapServer::instance = nullptr;

TrikCoapServer::TrikCoapServer(trikControl::BrickInterface &brick)
				: mBrick(brick) {
	TrikCoapServer::instance = this;
}

int TrikCoapServer::resolve_address(const char *host, const char *service, coap_address_t *dst) {
	struct addrinfo *res, *ainfo;
	struct addrinfo hints;
	int error, len=-1;

	memset(&hints, 0, sizeof(hints));
	memset(dst, 0, sizeof(*dst));
	hints.ai_socktype = SOCK_DGRAM;
	hints.ai_family = AF_UNSPEC;

	error = getaddrinfo(host, service, &hints, &res);

	if (error != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
		return error;
	}

	for (ainfo = res; ainfo != NULL; ainfo = ainfo->ai_next) {
		switch (ainfo->ai_family) {
		case AF_INET6:
		case AF_INET:
			len = dst->size = ainfo->ai_addrlen;
			memcpy(&dst->addr.sin6, ainfo->ai_addr, dst->size);
		default:
			;
		}
	}
	freeaddrinfo(res);
	return len;
}

void TrikCoapServer::init_sensors() {
	QVector<QString> sensorsPorts = {"A1","A2","A3","A4","A5","A6","D1","D2"};
	for (auto sensor : sensorsPorts) {
		sensors.push_back({sensor, mBrick.sensor(sensor)});
	}
}

void TrikCoapServer::init_motors() {
	QVector<QString> motorsPorts = {"M1","M2","M3","M4"};
	for (auto motor : motorsPorts) {
		motors.push_back({motor, mBrick.motor(motor)});
	}
}

template <int N>
void handler(coap_context_t *ctx,
			coap_resource_t *resource,
			coap_session_t *session,
			coap_pdu_t *request,
			coap_binary_t *token,
			coap_string_t *query,
			coap_pdu_t *response)
{
			(void)ctx;
			(void)query;
			auto sensor = TrikCoapServer::instance->sensors[N - 1];
			auto m = sensor.second->read();
			std::cout << sensor.first.toLatin1().data() << std::endl;
			std::cout << "AAAAAAAAAAAAAAAAAA" << std::endl;
			char buf[50];
			int len;
			len = sprintf(buf, "%d", m);
			response->code = COAP_RESPONSE_CODE(205);
			coap_add_data_blocked_response(resource, session, request, response, token,
								 COAP_MEDIATYPE_TEXT_PLAIN, 1,
								 len,
								 (unsigned char *)buf);
}

void motor_handler(coap_context_t *ctx,
			coap_resource_t *resource,
			coap_session_t *session,
			coap_pdu_t *request,
			coap_binary_t *token,
			coap_string_t *query,
			coap_pdu_t *response)
{
			(void)ctx;
			(void)resource;
			(void)session;
			(void)request;
			(void)token;
			(void)response;

			auto queryString = QString((char *) query->s);
			auto parts = queryString.split('=');
			auto motor = parts[0];
			auto power = parts[1].toInt();

			if (motor == "M1")
				TrikCoapServer::instance->motors[0].second->setPower(power);
			else if (motor == "M2")
				TrikCoapServer::instance->motors[1].second->setPower(power);
			else if (motor == "M3")
				TrikCoapServer::instance->motors[2].second->setPower(power);
			else
				TrikCoapServer::instance->motors[3].second->setPower(power);

			std::cout << "after set" << std::endl;

			response->code = COAP_RESPONSE_CODE(205);
			coap_add_data_blocked_response(resource, session, request, response, token,
											COAP_MEDIATYPE_TEXT_PLAIN, 1,
											4,
											(const uint8_t *)"done");
			std::cout << TrikCoapServer::instance->motors[0].second->power();
			std::cout << std::endl;
}

void led_handler(coap_context_t *ctx,
			coap_resource_t *resource,
			coap_session_t *session,
			coap_pdu_t *request,
			coap_binary_t *token,
			coap_string_t *query,
			coap_pdu_t *response)
{
			(void)ctx;
			(void)resource;
			(void)session;
			(void)request;
			(void)token;
			(void)response;

			auto led = TrikCoapServer::instance->mBrick.led();
			auto queryString = QString((char *) query->s);
			if (queryString == "green")
				led->green();
			else if (queryString == "red")
				led->red();
			else if (queryString == "orange")
				led->orange();
			else if (queryString == "off")
				led->off();

			response->code = COAP_RESPONSE_CODE(205);
			coap_add_data_blocked_response(resource, session, request, response, token,
											COAP_MEDIATYPE_TEXT_PLAIN, 1,
											4,
											(const uint8_t *)"done");
}

void TrikCoapServer::init_resources() {
	// handlers for each sensor
	QVector<coap_method_handler_t> handlers;
	handlers.push_back(handler<1>);
	handlers.push_back(handler<2>);
	handlers.push_back(handler<3>);
	handlers.push_back(handler<4>);
	handlers.push_back(handler<5>);
	handlers.push_back(handler<6>);
	handlers.push_back(handler<7>);
	handlers.push_back(handler<8>);

	for (auto i = 0; i < sensors.size(); i++) {
		auto sensor = sensors[i];
		coap_resource_t *r;
		auto sensor_name = sensor.first.toLatin1().data();
		size_t len = sensor.first.length();
		coap_str_const_t name{len, (const uint8_t *)sensor_name};
		std::cout << len << " " << sensor_name << std::endl;
		r = coap_resource_init(&name, COAP_RESOURCE_FLAGS_NOTIFY_NON);

		observable_resources.push_back(r);

		coap_register_handler(r, COAP_REQUEST_GET, handlers[i]);

		coap_resource_set_get_observable(r, 1);
		coap_add_resource(ctx, r);
	}

		coap_resource_t *r;
		coap_str_const_t name{6, (const uint8_t *)"motors"};
		r = coap_resource_init(&name, COAP_RESOURCE_FLAGS_NOTIFY_NON);
		coap_register_handler(r, COAP_REQUEST_GET, motor_handler);
		coap_add_resource(ctx, r);

		coap_resource_t *led;
		coap_str_const_t led_name{3, (const uint8_t *)"led"};
		led = coap_resource_init(&led_name, COAP_RESOURCE_FLAGS_NOTIFY_NON);
		coap_register_handler(led, COAP_REQUEST_GET, led_handler);
		coap_add_resource(ctx, led);
}

void TrikCoapServer::start(void) {
		init_sensors();
		init_motors();
		coap_set_log_level(LOG_DEBUG);
		timer = new QTimer();
		QObject::connect(timer, SIGNAL(timeout()), this, SLOT(coapRunOnce()));
		timer->setInterval(10);
		timer->start();

		coap_startup();

		/* resolve destination address where server should be sent */
		if (resolve_address("192.168.1.100", "5683", &dst) < 0) {
			coap_log(LOG_CRIT, "failed to resolve address\n");
			coap_free_context(this->ctx);
			coap_cleanup();
			return;
		}

		/* create CoAP context and a client session */
		ctx = coap_new_context(nullptr);

		if (!ctx || !(endpoint = coap_new_endpoint(ctx, &dst, COAP_PROTO_UDP))) {
			coap_log(LOG_EMERG, "cannot initialize context\n");
			coap_free_context(ctx);
			coap_cleanup();
			return;
		}

		init_resources();
	}

void TrikCoapServer::coapRunOnce() {
	coap_run_once(ctx, 0);
	for (auto resource : observable_resources)
		coap_resource_notify_observers(resource, NULL);
}
