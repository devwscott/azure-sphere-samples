#pragma once


#include "WifiStation.h" 
#include "MqttClient.h" 

#include "MQTTMessageCodecFactory.h"

using namespace std;


class Coordinator : public IWifiStationListener, public IMqttClientListener{
private:
    WifiStation *m_wifistation;
    MqttClient *m_mqttclient;
    MQTTMessageCodecFactory *m_mqttfactory;


public:
    Coordinator();
    virtual ~Coordinator();

    bool initialize();
    bool run();

    //TODO: need to be moved into private??
    bool _processMessage(string& topic, string& message);
    bool _generateMessage(string& topic, map<string, string>& payload);

    virtual void onNetworkConnected() override;
    virtual void onNetworkDisconnected() override;
    
    virtual void onReceiveTopic(string &topic, string &msg) override;
};


