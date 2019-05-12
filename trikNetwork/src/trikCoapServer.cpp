#include <QtCore/QDebug>

#include <QsLog.h>
#include "trikCoapServer.h"

using namespace trikNetwork;

//TrikCoapServer::TrikCoapServer(trikControl::BrickInterface &brick) : mBrick(brick){}

trikControl::BrickInterface* trikNetwork::TrikCoapServer::mBrick = nullptr;

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

void TrikCoapServer::init_resources() {
  coap_resource_t *r;

  coap_str_const_t name{4, (const uint8_t *)"time"};
  r = coap_resource_init(&name, COAP_RESOURCE_FLAGS_NOTIFY_CON);
  coap_register_handler(r, COAP_REQUEST_GET, hnd_get_distance);

  coap_resource_set_get_observable(r, 1);
  coap_add_resource(ctx, r);
  distance = r;
}

void TrikCoapServer::hnd_get_distance(coap_context_t *ctx,
					 coap_resource_t *resource,
					 coap_session_t *session,
					 coap_pdu_t *request,
					 coap_binary_t *token,
					 coap_string_t *query,
					 coap_pdu_t *response) {
	(void)ctx;
	(void)resource;
	(void)session;
	(void)request;
	(void)token;
	(void)query;
	(void)response;

	auto a3 = TrikCoapServer::mBrick->sensor("A3");
	auto data3 = a3->read();

	//SERIALIZATION?????
	char buf[50];
	int len;
	len = sprintf(buf, "%d", data3);
	response->code = COAP_RESPONSE_CODE(205);
	coap_add_data_blocked_response(resource, session, request, response, token,
								 COAP_MEDIATYPE_TEXT_PLAIN, 1,
								 len,
								 (unsigned char *)buf);
}

void TrikCoapServer::hnd_get_hello(coap_context_t *ctx,
					 coap_resource_t *resource,
					 coap_session_t *session,
					 coap_pdu_t *request,
					 coap_binary_t *token,
					 coap_string_t *query,
					 coap_pdu_t *response) {
	(void)ctx;
	(void)resource;
	(void)session;
	(void)request;
	(void)token;
	(void)query;

	response->code = COAP_RESPONSE_CODE(205);
//	auto a3 = TrikCoapServer::mBrick->sensor("A3");
//	auto a1 = TrikCoapServer::mBrick->sensor("A1");
//	auto a2 = TrikCoapServer::mBrick->sensor("A2");
//	auto a4 = TrikCoapServer::mBrick->sensor("A4");
//	auto a5 = TrikCoapServer::mBrick->sensor("A5");
//	auto a6 = TrikCoapServer::mBrick->sensor("A6");
//	auto data1 = a1->read();
//	auto data2 = a2->read();
//	auto data3 = a3->read();
//	auto data4 = a4->read();
//	auto data5 = a5->read();

	auto m1 = TrikCoapServer::mBrick->motor("M1");
	auto m2 = TrikCoapServer::mBrick->motor("M2");
	auto m3 = TrikCoapServer::mBrick->motor("M3");
	auto m4 = TrikCoapServer::mBrick->motor("M4");
	std::cout << m1->power() << std::endl;
	std::cout << m2->power() << std::endl;
	std::cout << m3->power() << std::endl;
	std::cout << m4->power() << std::endl;
	m1->setPower(30);
	m2->setPower(70);
	coap_add_data(response, 5, (const uint8_t *)"hello");
}

void TrikCoapServer::start(void) {
		coap_set_log_level(LOG_DEBUG);
		timer = new QTimer();
		QObject::connect(timer, SIGNAL(timeout()), this, SLOT(coapRunOnce()));
		timer->setInterval(1000);
		timer->start();

		coap_str_const_t ruri = { 5, (const uint8_t *)"hello" };

		coap_startup();

		/* resolve destination address where server should be sent */
		if (resolve_address("localhost", "5683", &dst) < 0) {
			coap_log(LOG_CRIT, "failed to resolve address\n");
			coap_free_context(this->ctx);
			coap_cleanup();
		}

		/* create CoAP context and a client session */
		ctx = coap_new_context(nullptr);

		if (!ctx || !(endpoint = coap_new_endpoint(ctx, &dst, COAP_PROTO_UDP))) {
			coap_log(LOG_EMERG, "cannot initialize context\n");
			coap_free_context(ctx);
			coap_cleanup();
		}

		resource = coap_resource_init(&ruri, 0);
		coap_register_handler(resource, COAP_REQUEST_GET, hnd_get_hello);
		coap_add_resource(ctx, resource);

		init_resources();
	}

void TrikCoapServer::coapRunOnce() {
	coap_run_once(ctx, 0);
	coap_resource_notify_observers(distance, NULL);
}
