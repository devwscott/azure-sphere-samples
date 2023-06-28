
#include <unistd.h>

#include <applibs/log.h>

#include "MQTTMessage.h"



MQTTMessage::MQTTMessage(string& topic, map<string, string>& payload):m_topic(topic), m_payload(payload){
    Log_Debug("MQTTMessage::MQTTMessage(topic, payload)\n");
}


MQTTMessage::MQTTMessage(string& topic, string& jsonstr):m_topic(topic), m_jsonstr(jsonstr){
    Log_Debug("MQTTMessage::MQTTMessage(topic, jsonstr)\n");
}


MQTTMessage::~MQTTMessage(){
    Log_Debug("MQTTMessage::~MQTTMessage()\n");
}


string MQTTMessage::getTopic(){
    Log_Debug("MQTTMessage::getTopic()\n");
    return m_topic;
}


map<string, string> MQTTMessage::getPayload(){
    Log_Debug("MQTTMessage::getPayload()\n");
    return m_payload;
}


string MQTTMessage::toJson(){
    Log_Debug("MQTTMessage::toJson()\n");
    return m_jsonstr;
}

