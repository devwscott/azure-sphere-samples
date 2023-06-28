
#include <unistd.h>

#include <applibs/log.h>

#include "MQTTMessageCodecFactoryImpl.h"
#include "MQTTMessageLightCodec.h"



MQTTMessageCodecFactoryImpl::MQTTMessageCodecFactoryImpl(){
    Log_Debug("MQTTMessageCodecFactoryImpl::MQTTMessageCodecFactoryImpl()\n");

}


MQTTMessageCodecFactoryImpl::~MQTTMessageCodecFactoryImpl(){
    Log_Debug("MQTTMessageCodecFactoryImpl::~MQTTMessageCodecFactoryImpl()\n");

}

MQTTMessageCodec* MQTTMessageCodecFactoryImpl::createCodec(string& topic){
    Log_Debug("MQTTMessageCodecFactoryImpl::createCodec()\n");

    Log_Debug("[MQTTMessageCodecFactoryImpl::createCodec] topic is %s\n", topic.c_str());

    if(topic.find("light") != string::npos){
        return new MQTTMessageLightCodec(topic);
    }
    else if(topic.find("xxx") != string::npos){
        // return new MQTTMessageXXXCodec(topic);
    }

    return NULL;
    
}



