
#include <unistd.h>
#include <sstream>

#include <applibs/log.h>

#include "MQTTLightControlMessage.h"
#include "parson.h"


MQTTLightControlMessage::MQTTLightControlMessage(string& topic, string& jsonstr):MQTTMessage(topic, jsonstr){
    Log_Debug("MQTTLightControlMessage::MQTTLightControlMessage()\n");

    //TODO: parsing jsonstr with JSON lib
    //      fill the map data
    //      set map data into MQTTMessage::m_payload

    map<string, string> payload;
    JSON_Value *root;
    JSON_Object *object;

    const char* str_value;
    int int_value;

// {
//   "type":"Light",
//   "control":"on",
//   "desired": {
//       "id" : 1234,
//       "state" : "on"
//   }
// }

    root = json_parse_string(jsonstr.c_str());
    if(json_value_get_type(root) != JSONObject){
        if(root != NULL){
            json_value_free(root);
        }
        Log_Debug("[MQTTLightControlMessage::MQTTLightControlMessage] error : json_parse_string()\n");
    }

    object = json_value_get_object(root);
    if(object != NULL){
        if((str_value = json_object_get_string(object, "type")) != NULL){
            payload["type"] = string(str_value);
        }
        
        if((str_value = json_object_get_string(object, "control")) != NULL){
            payload["control"] = string(str_value);
        }

        if((int_value = json_object_dotget_number(object, "desired.id")) != 0){
            stringstream ss;
            ss << int_value;
            payload["desired.id"] = ss.str();
            
            // payload["desired.id"] = to_string(int_value);
        }

        if((str_value = json_object_dotget_string(object, "desired.state")) != NULL){
            payload["desired.state"] = string(str_value);
        }
    }
    
    // MQTTMessage::m_payload = payload;
    m_payload = payload;
}



MQTTLightControlMessage::~MQTTLightControlMessage(){
    Log_Debug("MQTTLightControlMessage::~MQTTLightControlMessage()\n");

}

