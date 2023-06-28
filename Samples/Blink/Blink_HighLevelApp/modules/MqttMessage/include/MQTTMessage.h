#pragma once

#include <pthread.h>
#include <string>
#include <map>

using namespace std;


class MQTTMessage{
protected:
    string m_topic;
    map<string, string> m_payload;
    string m_jsonstr;


public:
    MQTTMessage(string& topic, map<string,string>& payload);
    MQTTMessage(string& topic, string& jsonstr);

    virtual ~MQTTMessage();
    string getTopic();
    map<string, string> getPayload();
    string toJson();

};

