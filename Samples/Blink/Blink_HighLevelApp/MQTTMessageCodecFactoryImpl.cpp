
#include <unistd.h>

#include <applibs/log.h>

#include "MQTTMessageCodecFactoryImpl.h"

// class MQTTMessageCodecFactoryImpl : public MQTTMessageCodecFactory{
// public:
//     MQTTMessageCodecFactoryImpl();
//     virtual ~MQTTMessageCodecFactoryImpl();
//     virtual MQTTMessageCodec* createCodec(string& topic) override;
// };


MQTTMessageCodecFactoryImpl::MQTTMessageCodecFactoryImpl(){
    Log_Debug("MQTTMessageCodecFactoryImpl::MQTTMessageCodecFactoryImpl()\n");

}


MQTTMessageCodecFactoryImpl::~MQTTMessageCodecFactoryImpl(){
    Log_Debug("MQTTMessageCodecFactoryImpl::~MQTTMessageCodecFactoryImpl()\n");

}

MQTTMessageCodec* MQTTMessageCodecFactoryImpl::createCodec(string& topic){
    Log_Debug("MQTTMessageCodecFactoryImpl::createCodec()\n");

}



