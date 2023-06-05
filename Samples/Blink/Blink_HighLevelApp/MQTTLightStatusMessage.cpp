
#include <unistd.h>
#include <cstdlib>

#include <applibs/log.h>

#include "MQTTLightStatusMessage.h"
#include "parson.h"

MQTTLightStatusMessage::MQTTLightStatusMessage(string& topic, map<string,string>& payload):MQTTMessage(topic, payload){
    Log_Debug("MQTTLightStatusMessage::MQTTLightStatusMessage()\n");

    //TODO: convert payload(map data) to JsonObject
    //      convert JsonObject to string 
    //      set jsonstr to MQTTMessage::m_jsonstr

    JSON_Value *root;
    JSON_Object *object;
    char *serialized_str;

// {
//     "type": "Light",
//     "status": "on",
//     "reported": {
//         "id": 1234,
//         "state": "on"
//     }
// }

    root = json_value_init_object();
    if(root != NULL){
        object = json_value_get_object(root);
        if(object != NULL){
            json_object_set_string(object, "type", "Light");
            json_object_set_string(object, "status", "on");

            json_object_dotset_number(object, "reported.id", atoi(payload["id"].c_str()));
            // json_object_dotset_number(object, "reported.id", stoi(payload["id"]));

            json_object_dotset_string(object, "reported.state", payload["state"].c_str());

            serialized_str = json_serialize_to_string_pretty(root);
            if(serialized_str == NULL){
                Log_Debug("[MQTTLightStatusMessage::MQTTLightStatusMessage] error : json_serialize_to_string_pretty()\n");
            }
        }
    }
    
    // Test jsonstr
    // string jsonstr("{\"id\":1234,\"status\":\"on\"}");
    
    string jsonstr(serialized_str);
    
    MQTTMessage::m_jsonstr = jsonstr;
   
    json_free_serialized_string(serialized_str);
    json_value_free(root);
}


MQTTLightStatusMessage::~MQTTLightStatusMessage(){
    Log_Debug("MQTTLightStatusMessage::~MQTTLightStatusMessage()\n");

}

