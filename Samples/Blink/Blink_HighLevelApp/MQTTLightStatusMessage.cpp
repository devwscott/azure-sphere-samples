
#include <unistd.h>

#include <applibs/log.h>

#include "MQTTLightStatusMessage.h"


MQTTLightStatusMessage::MQTTLightStatusMessage(string& topic, map<string,string>& payload):MQTTMessage(topic, payload){
    Log_Debug("MQTTLightStatusMessage::MQTTLightStatusMessage()\n");

    //TODO: convert payload(map data) to JsonObject
    //      convert JsonObject to string 
    //      set jsonstr to MQTTMessage::m_jsonstr

    // Test jsonstr
    string jsonstr("{\"id\":1234,\"status\":\"on\"}");

    MQTTMessage::m_jsonstr = jsonstr;
}


MQTTLightStatusMessage::~MQTTLightStatusMessage(){
    Log_Debug("MQTTLightStatusMessage::~MQTTLightStatusMessage()\n");

}

