#pragma once

#include <pthread.h>
#include <string>

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

public:
    MqttClient(IMqttClientListener *callback);
    virtual ~MqttClient();

    bool init(int qosLevel, int keepAliveTime);
    bool connect(string &broker, string &port, string &certFile);
    bool subscribe(string &topic);
    bool unSubscribe(string &topic);
    bool publish(string &topic, string &msg);
};

