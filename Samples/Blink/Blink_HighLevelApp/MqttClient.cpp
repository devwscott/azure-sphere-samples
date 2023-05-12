
#include <applibs/log.h>

#include "MqttClient.h"



MqttClient::MqttClient(IMqttClientListener *callback):m_running(false) {
    Log_Debug("MqttClient::MqttClient()\n");

}



MqttClient::~MqttClient(){
    Log_Debug("MqttClient::~MqttClient()\n");
}



bool MqttClient::init(int qoslevel, int keepAliveTime){
    Log_Debug("MqttClient::init()\n");
    
    return true;
}



bool MqttClient::connect(string &broker, string &port, string &certFile){
    Log_Debug("MqttClient::connect()\n");

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

