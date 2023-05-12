
#include <unistd.h>
#include <applibs/log.h>

#include "MqttClient.h"



MqttClient::MqttClient(IMqttClientListener *callback):m_running(false) {
    Log_Debug("MqttClient::MqttClient()\n");

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



bool MqttClient::connect(const string &broker, const string &port, const string &certFile){
    Log_Debug("MqttClient::connect()\n");

    Log_Debug("     broker:%s, port:%s, certFile:%s \n", broker.c_str(), port.c_str(), certFile.c_str());

    return true;
}



bool MqttClient::subscribe(string &topic){
    Log_Debug("MqttClient::subscribe()\n");

    return true;
}



bool MqttClient::unSubscribe(string &topic){
    Log_Debug("MqttClient::unSubscribe()\n");
    return true;
}



bool MqttClient::publish(string &topic, string &msg){
    Log_Debug("MqttClient::publish()\n");
    return true;
}



void* MqttClient::__syncThread__(void *arg){
    MqttClient *pMqttClient = static_cast<MqttClient*>(arg);

    int runInterval = 500*1000;     //500ms sleep()

    Log_Debug("MqttClient::__syncThread__()\n");

    while(1){

        Log_Debug("[MqttClient::__syncThread__] running\n");
       
        if(pMqttClient->m_running){

        }

        usleep(runInterval);
    }

    return nullptr;
}


