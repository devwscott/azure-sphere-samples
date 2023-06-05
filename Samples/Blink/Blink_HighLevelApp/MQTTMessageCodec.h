#pragma once

#include <string>
#include <map>

#include "MQTTMessage.h"

using namespace std;


class MQTTMessageCodec{
protected:
    string m_topic;

public:
    virtual MQTTMessage* decode(string& message) = 0;
    virtual MQTTMessage* encode(map<string, string>& payload) = 0;
};

