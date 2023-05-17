
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>

#include <mbedtls/error.h>
#include <mbedtls/entropy.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/net_sockets.h>
#include <mbedtls/ssl.h>
#include <sys/random.h>
#include <applibs/storage.h>

#include <applibs/log.h>
#include "MqttClient.h"

#include "mqtt.h"


typedef struct _mbedtls_context {
    mbedtls_net_context net_ctx;
    mbedtls_ssl_context ssl_ctx;
    mbedtls_ssl_config ssl_conf;
    mbedtls_x509_crt ca_crt;
    mbedtls_entropy_context entropy;
    mbedtls_ctr_drbg_context ctr_drbg;
}mbedtls_context;



MqttClient::MqttClient(IMqttClientListener *callback):m_running(false), m_socketFd(-1) {
    Log_Debug("MqttClient::MqttClient()\n");

    m_listener = callback;
    m_mbedtls_ctx = new mbedtls_context;
}



MqttClient::~MqttClient(){
    Log_Debug("MqttClient::~MqttClient()\n");
    delete m_mbedtls_ctx;
}



bool MqttClient::init(int qoslevel, int keepAliveTime){
    bool ret = true;
    Log_Debug("MqttClient::init()\n");

    m_qosLevel = qoslevel;
    m_keepAliveTime = keepAliveTime;

    if(pthread_create(&m_thread, NULL, MqttClient::__syncThread__, this) != 0){
        Log_Debug("[MqttClient::init] error : pthread_create()\n");
        ret = false;
    }

    return ret;
}



bool MqttClient::connect(const string &broker, const string &port){
#if !defined(MQTT_USE_MBEDTLS)
    Log_Debug("MqttClient::connect() without CA Cert\n");

    Log_Debug("     broker:%s, port:%s\n", broker.c_str(), port.c_str());

	m_socketFd = _open_nb_socket(broker, port);

	if (m_socketFd == -1) {
        Log_Debug("[MqttClient::connect] error : _open_nb_socket()\n");
        return false;
	}

    /* store this pointer */
    m_client.publish_response_callback_state = this;

	if(mqtt_init(&m_client, m_socketFd, m_sendbuf, sizeof(m_sendbuf), m_recvbuf, sizeof(m_recvbuf), MqttClient::__subscribe_callback__) != MQTT_OK){
        Log_Debug("[MqttClient::connect] error : mqtt_init()\n");
        return false;
    }

	// if(mqtt_connect(&m_client, "device_name_id", NULL, NULL, 0, "esp32", "1234", MQTT_CONNECT_CLEAN_SESSION, m_keepAliveTime) != MQTT_OK){
	if(mqtt_connect(&m_client, NULL, NULL, NULL, 0, "esp32", "1234", MQTT_CONNECT_CLEAN_SESSION, m_keepAliveTime) != MQTT_OK){
        Log_Debug("[MqttClient::connect] error : mqtt_connect()\n");
        return false;
    }

    /* For, Testing. sub Topic */
    string sub = "hello_mt";
    subscribe(sub);


    m_running = true;
#endif

    return true;
}



bool MqttClient::connect(const string &broker, const string &port, const string &certFile){
#if defined(MQTT_USE_MBEDTLS)
    mqtt_pal_socket_handle socketFd;
    mbedtls_context *ctx = (mbedtls_context*)m_mbedtls_ctx;
    
    Log_Debug("MqttClient::connect() with CA Cert\n");
    Log_Debug("     broker:%s, port:%s, certFile:%s \n", broker.c_str(), port.c_str(), certFile.c_str());

	if(_open_nb_socket(broker, port, certFile) != 0){
        Log_Debug("[MqttClient::connect] error : _open_nb_socket()\n");
		return false;
    }

    socketFd = &ctx->ssl_ctx;

    if (socketFd == NULL) {
        Log_Debug("[MqttClient::connect] error : _open_nb_socket(), socketFd is NULL\n");
		return false;
	}

    m_socketFd = reinterpret_cast<int>(socketFd);

    /* store this pointer */
    m_client.publish_response_callback_state = this;

	if(mqtt_init(&m_client, socketFd, m_sendbuf, sizeof(m_sendbuf), m_recvbuf, sizeof(m_recvbuf), MqttClient::__subscribe_callback__) != MQTT_OK){
        Log_Debug("[MqttClient::connect] error : mqtt_init()\n");
        return false;
    }

	if(mqtt_connect(&m_client, "New_MT3620_with_MbedTLS", NULL, NULL, 0, NULL, NULL, MQTT_CONNECT_CLEAN_SESSION, m_keepAliveTime) != MQTT_OK){
        Log_Debug("[MqttClient::connect] error : mqtt_connect()\n");
        return false;
    }

    /* For, Testing. sub Topic */
    string sub = "hello_mt";
    subscribe(sub);

    m_running = true;

#endif
    return true;
}



bool MqttClient::disConnect(){
    Log_Debug("MqttClient::disConnect()\n");

	if(mqtt_disconnect(&m_client) != MQTT_OK){
        Log_Debug("[MqttClient::disConnect] error : mqtt_disConnect()\n");
        return false;
    }

#if defined(MQTT_USE_MBEDTLS)
    mbedtls_context *ctx = (mbedtls_context*)m_mbedtls_ctx;
    mbedtls_net_context *net_ctx = &ctx->net_ctx;
    mbedtls_ssl_context *ssl_ctx = &ctx->ssl_ctx;
    mbedtls_ssl_config *ssl_conf = &ctx->ssl_conf;
    mbedtls_x509_crt *ca_crt = &ctx->ca_crt;
    mbedtls_entropy_context *entropy = &ctx->entropy;
    mbedtls_ctr_drbg_context *ctr_drbg = &ctx->ctr_drbg;

    mbedtls_net_free(net_ctx);
    mbedtls_x509_crt_free(ca_crt);
    mbedtls_ssl_free(ssl_ctx);
    mbedtls_ssl_config_free(ssl_conf);
    mbedtls_ctr_drbg_free(ctr_drbg);
    mbedtls_entropy_free(entropy);
#else
	if (m_socketFd != -1) {
		close(m_socketFd);
	}
#endif
	m_socketFd = -1;
    m_running = false;

    return true;
}



bool MqttClient::subscribe(string &topic){
    Log_Debug("MqttClient::subscribe()\n");

    MQTTPublishFlags qos;
    qos = m_qosLevel==2?MQTT_PUBLISH_QOS_2:m_qosLevel==1?MQTT_PUBLISH_QOS_1:MQTT_PUBLISH_QOS_0;
    Log_Debug("[MqttClient::subscribe] qos level = 0x%x\n", qos);
 
    // mqtt_subscribe(&m_client, topic.c_str(), MQTT_PUBLISH_QOS_1);
    if(mqtt_subscribe(&m_client, topic.c_str(), qos) != MQTT_OK){
        Log_Debug("[MqttClient::subscribe] error : mqtt_subscribe()\n");
        return false;
    }

    return true;
}



bool MqttClient::unSubscribe(string &topic){
    Log_Debug("MqttClient::unSubscribe()\n");

    if(mqtt_unsubscribe(&m_client, topic.c_str()) != MQTT_OK){
        Log_Debug("[MqttClient::unSubscribe] error : mqtt_unsubscribe()\n");
        return false;
    }

    return true;
}



bool MqttClient::publish(string &topic, string &msg){
    Log_Debug("MqttClient::publish()\n");

    MQTTPublishFlags qos;
    qos = m_qosLevel==2?MQTT_PUBLISH_QOS_2:m_qosLevel==1?MQTT_PUBLISH_QOS_1:MQTT_PUBLISH_QOS_0;
    Log_Debug("[MqttClient::publish] qos level = 0x%x\n", qos);

    // mqtt_publish(&client, topic, msg, strlen(msg) + 1, MQTT_PUBLISH_QOS_1);
    if(mqtt_publish(&m_client, topic.c_str(), msg.c_str(), msg.size(), qos) != MQTT_OK){
        Log_Debug("[MqttClient::publish] error : mqtt_publish()\n");
        return false;
    }


    return true;
}



void MqttClient::__subscribe_callback__(void **arg, struct mqtt_response_publish* published){
    MqttClient *pMqttClient = static_cast<MqttClient*>(*arg);

/*
	char* topic = malloc(published->topic_name_size + 1);
	memcpy(topic, published->topic_name, published->topic_name_size);
	topic[published->topic_name_size] = '\0';

	char* msg = malloc(published->application_message_size + 1);
	memcpy(msg, published->application_message, published->application_message_size);
	msg[published->application_message_size] = '\0';
*/

    /* convert (const void*) to (char*) */
    char *_topic = const_cast<char*>(static_cast<const char*>(published->topic_name));
    char *_msg = const_cast<char*>(static_cast<const char*>(published->application_message));

    string topic(_topic, _topic + published->topic_name_size);
    string msg(_msg, _msg + published->application_message_size);

    pMqttClient->m_listener->onReceiveTopic(topic, msg);

    return;
}



int MqttClient::_open_nb_socket(const string &addr, const string &port) {
	struct addrinfo hints = { 0 };

	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	int sockfd = -1;
	int rv;
	struct addrinfo *servinfo;


	rv = getaddrinfo(addr.c_str(), port.c_str(), &hints, &servinfo);
	if (rv == -1) {
		return -1;
	}

	sockfd = socket(servinfo->ai_family, hints.ai_socktype | SOCK_NONBLOCK, servinfo->ai_protocol);
	if (sockfd == -1) {
		return -1;
	}

	// rv = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
	rv = ::connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    if(rv==-1 && errno != EINPROGRESS){
        Log_Debug("error : socket connect()\n");
    }

	struct pollfd pfd = {
		.fd = sockfd,
		.events = 255,
		.revents = 255
	};

	// int res = poll(&pfd, 1, 500);// wait for an event from socket
	int res = poll(&pfd, 1, 1000);// wait for an event from socket
	if (res <= 0) {
		Log_Debug("\nNo response from socket\n");
		close(sockfd);
		return -1;
	}

	freeaddrinfo(servinfo);

	return sockfd;
}


//TODO: need to be checked!! moved into class as private member function?
void failed(const char *fn, int rv) {
    char buf[100];
    mbedtls_strerror(rv, buf, sizeof(buf));
    Log_Debug("%s failed with %x (%s)\n", fn, -rv, buf);
    // exit(1);
}


//TODO: need to be checked!! moved into class as private member function?
void cert_verify_failed(uint32_t rv) {
    char buf[512];
    mbedtls_x509_crt_verify_info(buf, sizeof(buf), "\t", rv);
    Log_Debug("Certificate verification failed (%0" PRIx32 ")\n%s\n", rv, buf);
    // exit(1);
}


//TODO: need to be checked!! moved into class as private static member function as pthread handler()??
// this function located at m_mbedtls_ctx->entropy
int _entropy_pluton_source(void* data, unsigned char* output,
	size_t len, size_t* olen)
{
	ssize_t bytes_copied;
	((void)data);

	// getrandom is added after 19.05 to levearge pluton's TRNG
	bytes_copied = getrandom(output, len, 0);
	if (bytes_copied >= 0) {
		*olen = bytes_copied;
		return 0;
	} else {
		return MBEDTLS_ERR_ENTROPY_SOURCE_FAILED;
	}
}


int MqttClient::_open_nb_socket(const string &addr, const string &port, const string &cert) {
    Log_Debug("MqttClient::_open_nb_socket() with CA Cert\n");

    int rv;
    int ret;
    const char* pers = "azure sphere SSL client";

    mbedtls_context *ctx = (mbedtls_context*)m_mbedtls_ctx;
    mbedtls_net_context *net_ctx = &ctx->net_ctx;
    mbedtls_ssl_context *ssl_ctx = &ctx->ssl_ctx;
    mbedtls_ssl_config *ssl_conf = &ctx->ssl_conf;
    mbedtls_x509_crt *ca_crt = &ctx->ca_crt;
    mbedtls_entropy_context *entropy = &ctx->entropy;
    mbedtls_ctr_drbg_context *ctr_drbg = &ctx->ctr_drbg;

    mbedtls_entropy_init(entropy);
    mbedtls_ctr_drbg_init(ctr_drbg);

#if 1
	mbedtls_entropy_add_source(entropy, _entropy_pluton_source, NULL, MBEDTLS_ENTROPY_MAX_GATHER, MBEDTLS_ENTROPY_SOURCE_STRONG);

	if ((ret = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy,
		(const unsigned char*)pers,
		strlen(pers))) != 0)
	{
		Log_Debug(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n", ret);
		// goto exit;
        return -1;
	}

	Log_Debug(" ok\n");
#else
    rv = mbedtls_ctr_drbg_seed(ctr_drbg, mbedtls_entropy_func, entropy,
                               additional, additional_len);
    if (rv != 0) {
        failed("mbedtls_ctr_drbg_seed", rv);
    }
#endif

    mbedtls_x509_crt_init(ca_crt);

#if 1
	/*
	 * 0. Initialize certificates
	 */
	Log_Debug("  . Loading the CA root certificate ...");
	fflush(stdout);

	ret = Storage_OpenFileInImagePackage(cert.c_str());
	if (ret < 0) {
		Log_Debug(" failed\n  !  Storage_OpenFileInImagePackage fail\n\n");
		// goto exit;
        return -1;
	}

	int cert_fd = ret;

	// char *p_cert_file = malloc(8192); // certificate will not exceed 8KB
	unsigned char *p_cert_file = new unsigned char[8192]; // certificate will not exceed 8KB
	if (p_cert_file == NULL) {
		Log_Debug(" failed\n  !  malloc fail\n\n");
		close(cert_fd);
		// goto exit;
        return -1;
	}

	ret = read(cert_fd, p_cert_file, 8192);
	if (ret < 0) {
		Log_Debug(" failed\n  !  read fail\n\n");
		close(cert_fd);
		// free(p_cert_file);
        delete[] p_cert_file;
		// goto exit;
        return -1;
	}

	ret = mbedtls_x509_crt_parse_der(ca_crt, p_cert_file, ret);
	if (ret < 0) {
		Log_Debug(" failed\n  !  mbedtls_x509_crt_parse_der returned -0x%x\n\n", -ret);
		close(cert_fd);
		// free(p_cert_file);
        delete[] p_cert_file;
		// goto exit;
        return -1;
	}

	// free(p_cert_file);
    delete[] p_cert_file;
	close(cert_fd);

	Log_Debug(" ok (%d skipped)\n", ret);
#else
    rv = mbedtls_x509_crt_parse_file(ca_crt, ca_file);
    if (rv != 0) {
        failed("mbedtls_x509_crt_parse_file", rv);
    }
#endif

    mbedtls_ssl_config_init(ssl_conf);
    rv = mbedtls_ssl_config_defaults(ssl_conf,  MBEDTLS_SSL_IS_CLIENT,
                                     MBEDTLS_SSL_TRANSPORT_STREAM,
                                     MBEDTLS_SSL_PRESET_DEFAULT);
    if (rv != 0) {
        failed("mbedtls_ssl_config_defaults", rv);
        return -1;
    }
    mbedtls_ssl_conf_ca_chain(ssl_conf, ca_crt, NULL);
    mbedtls_ssl_conf_authmode(ssl_conf, MBEDTLS_SSL_VERIFY_OPTIONAL);
    mbedtls_ssl_conf_rng(ssl_conf, mbedtls_ctr_drbg_random, ctr_drbg);

    mbedtls_net_init(net_ctx);
    rv = mbedtls_net_connect(net_ctx, addr.c_str(), port.c_str(), MBEDTLS_NET_PROTO_TCP);
    if (rv != 0) {
        failed("mbedtls_net_connect", rv);
        return -1;
    }

    rv = mbedtls_net_set_nonblock(net_ctx);
    if (rv != 0) {
        failed("mbedtls_net_set_nonblock", rv);
        return -1;
    }

    mbedtls_ssl_init(ssl_ctx);
    rv = mbedtls_ssl_setup(ssl_ctx, ssl_conf);
    if (rv != 0) {
        failed("mbedtls_ssl_setup", rv);
        return -1;
    }

#if 1
    // It's normal case
    // rv = mbedtls_ssl_set_hostname(ssl_ctx, addr.c_str());
#else
    // Just for testing, without CN(Common Name) dependency
    rv = mbedtls_ssl_set_hostname(ssl_ctx, "192.168.45.197");
#endif
    if (rv != 0) {
        failed("mbedtls_ssl_set_hostname", rv);
        return -1;
    }

    mbedtls_ssl_set_bio(ssl_ctx, net_ctx,
                        mbedtls_net_send, mbedtls_net_recv, NULL);

    Log_Debug("  . Performing the SSL/TLS handshake...");

    for (;;) {
        rv = mbedtls_ssl_handshake(ssl_ctx);
        uint32_t want = 0;
        if (rv == MBEDTLS_ERR_SSL_WANT_READ) {
            want |= MBEDTLS_NET_POLL_READ;
        } else if (rv == MBEDTLS_ERR_SSL_WANT_WRITE) {
            want |= MBEDTLS_NET_POLL_WRITE;
        } else {
            break;
        }
        rv = mbedtls_net_poll(net_ctx, want, -1);
        if (rv < 0) {
            failed("mbedtls_net_poll", rv);
        }
    }

    if (rv != 0) {
        failed("mbedtls_ssl_handshake", rv);
        return -1;
    }

    uint32_t result = mbedtls_ssl_get_verify_result(ssl_ctx);
    if (result != 0) {
        if (result == (uint32_t)-1) {
            failed("mbedtls_ssl_get_verify_result", result);
        } else {
            cert_verify_failed(result);
        }
        return -1;
    }

    return 0;
}





void* MqttClient::__syncThread__(void *arg){
    MqttClient *pMqttClient = static_cast<MqttClient*>(arg);

    int runInterval = 100*1000;     //100ms sleep()

    Log_Debug("MqttClient::__syncThread__()\n");

    while(1){

        Log_Debug("[MqttClient::__syncThread__] running\n");

        if(pMqttClient->m_running){

            if(mqtt_sync((struct mqtt_client*)&pMqttClient->m_client) != MQTT_OK){
                Log_Debug("[MqttClient::__syncThread__] error : mqtt_sync()\n");
            }

        }

        usleep(runInterval);
    }

    return nullptr;
}


