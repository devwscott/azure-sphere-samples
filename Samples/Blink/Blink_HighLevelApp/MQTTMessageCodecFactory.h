#pragma once

#include <string>
#include <map>

#include "MQTTMessageCodec.h"

using namespace std;


class MQTTMessageCodecFactory{
public:
    virtual MQTTMessageCodec* createCodec(string& topic) = 0;
};

