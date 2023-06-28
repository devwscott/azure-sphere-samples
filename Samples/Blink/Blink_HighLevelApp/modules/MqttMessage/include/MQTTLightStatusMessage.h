#pragma once

#include <pthread.h>
#include <string>

#include "MQTTMessage.h"

using namespace std;


class MQTTLightStatusMessage : public MQTTMessage{
public:
    MQTTLightStatusMessage(string& topic, map<string,string>& payload);
    virtual ~MQTTLightStatusMessage();
};

