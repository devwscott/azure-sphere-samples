#pragma once

#include <pthread.h>
#include <string>

#include "MQTTMessage.h"

using namespace std;

class MQTTLightControlMessage : public MQTTMessage{
public:
    MQTTLightControlMessage(string& topic, string& jsonstr);
    virtual ~MQTTLightControlMessage();
};


