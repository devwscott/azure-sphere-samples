
#include <unistd.h>

#include <applibs/log.h>

#include "MQTTLightControlMessage.h"



MQTTLightControlMessage::MQTTLightControlMessage(string& topic, string& jsonstr):MQTTMessage(topic, jsonstr){
    Log_Debug("MQTTLightControlMessage::MQTTLightControlMessage()\n");

    //TODO: parsing jsonstr with JSON lib
    //      fill the map data
    //      set map data into MQTTMessage::m_payload

    map<string, string> payload;

    //test map data
    payload["k1"] = "v1";
    payload["k2"] = "v2";
    payload["k3"] = "v3";

    MQTTMessage::m_payload = payload;
}



MQTTLightControlMessage::~MQTTLightControlMessage(){
    Log_Debug("MQTTLightControlMessage::~MQTTLightControlMessage()\n");

}

