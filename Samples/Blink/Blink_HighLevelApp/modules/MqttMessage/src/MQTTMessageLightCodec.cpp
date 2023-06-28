
#include <unistd.h>

#include <applibs/log.h>

#include "MQTTMessageLightCodec.h"
#include "MQTTLightControlMessage.h"
#include "MQTTLightStatusMessage.h"


MQTTMessageLightCodec::MQTTMessageLightCodec(string& topic){
    Log_Debug("MQTTMessageLightCodec::MQTTMessageLightCodec(topic)\n");
    m_topic = topic;

}


MQTTMessageLightCodec::~MQTTMessageLightCodec(){
    Log_Debug("MQTTMessageLightCodec::~MQTTMessageLightCodec()\n");

}


MQTTMessage* MQTTMessageLightCodec::decode(string& message){
    Log_Debug("MQTTMessageLightCodec::decode()\n");
    Log_Debug("m_topic is %s\n", m_topic.c_str());

    if(m_topic == "light_control"){
        return new MQTTLightControlMessage(m_topic, message);
    }
    else if(m_topic == "xxx_xxx"){
        Log_Debug("m_topic is xxx_xxx! \n");
    }

    return NULL;
}


MQTTMessage* MQTTMessageLightCodec::encode(map<string,string>& payload){
    Log_Debug("MQTTMessageLightCodec::encode()\n");
    Log_Debug("m_topic is %s\n", m_topic.c_str());

    if(m_topic == "light_status"){
        return new MQTTLightStatusMessage(m_topic, payload);
    }
    else if(m_topic == "xxx_xxx"){
        Log_Debug("m_topic is xxx_xxx!\n");
    }

    return NULL;
}



