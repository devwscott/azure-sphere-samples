#pragma once

#include <string>
#include <map>

#include "MQTTMessageCodec.h"

using namespace std;


class MQTTMessageLightCodec : public MQTTMessageCodec{
public:
    MQTTMessageLightCodec(string& topic);
    virtual ~MQTTMessageLightCodec();

    virtual MQTTMessage* decode(string& message) override; 
    virtual MQTTMessage* encode(map<string, string>& payload) override;
};

