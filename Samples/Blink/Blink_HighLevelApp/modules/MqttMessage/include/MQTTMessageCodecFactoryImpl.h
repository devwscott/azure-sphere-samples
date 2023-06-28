#pragma once

#include <string>
#include <map>

#include "MQTTMessageCodecFactory.h"

using namespace std;


class MQTTMessageCodecFactoryImpl : public MQTTMessageCodecFactory{
public:
    MQTTMessageCodecFactoryImpl();
    virtual ~MQTTMessageCodecFactoryImpl();
    virtual MQTTMessageCodec* createCodec(string& topic) override;
};

