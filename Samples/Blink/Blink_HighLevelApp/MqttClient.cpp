
#include <unistd.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <poll.h>
#include <errno.h>

#include <applibs/log.h>
#include "MqttClient.h"

#include "mqtt.h"



MqttClient::MqttClient(IMqttClientListener *callback):m_running(false) {
    Log_Debug("MqttClient::MqttClient()\n");

    m_listener = callback;

}



MqttClient::~MqttClient(){
    Log_Debug("MqttClient::~MqttClient()\n");
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

    return true;
}



bool MqttClient::connect(const string &broker, const string &port, const string &certFile){
    Log_Debug("MqttClient::connect() with CA Cert\n");
    Log_Debug("     broker:%s, port:%s, certFile:%s \n", broker.c_str(), port.c_str(), certFile.c_str());

    return true;
}



bool MqttClient::disConnect(){
    Log_Debug("MqttClient::disConnect()\n");

	if(mqtt_disconnect(&m_client) != MQTT_OK){
        Log_Debug("[MqttClient::disConnect] error : mqtt_disConnect()\n");
        return false;
    }

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


