#pragma once

#include <pthread.h>
#include <string>

#include "mqtt.h"

using namespace std;

class IMqttClientListener{
public:
    virtual void onReceiveTopic(string &topic, string &msg) = 0;
};


class MqttClient{
private:
    int m_qosLevel;
    int m_keepAliveTime;
    IMqttClientListener *m_listener;
    bool m_running;
    pthread_t m_thread;
    static void* __syncThread__(void *arg);

    int m_socketFd;
    // void *m_client;
    mqtt_client m_client;
    unsigned char m_sendbuf[512];
    unsigned char m_recvbuf[512];
    int _open_nb_socket(const string &addr, const string &port);

    void *m_mbedtls_ctx;
    int _open_nb_socket(const string &addr, const string &port, const string &cert);

    static void __subscribe_callback__(void **arg, struct mqtt_response_publish* published);

public:
    MqttClient(IMqttClientListener *callback);
    virtual ~MqttClient();

    bool init(int qosLevel, int keepAliveTime);
    bool connect(const string &broker, const string &port);
    bool connect(const string &broker, const string &port, const string &certFile);
    bool disConnect();
    bool subscribe(string &topic);
    bool unSubscribe(string &topic);
    bool publish(string &topic, string &msg);
};

