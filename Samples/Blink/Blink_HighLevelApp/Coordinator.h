#pragma once


#include "WifiStation.h" 
#include "MqttClient.h" 
// #include "MQTTMessage.h"
#include "MQTTLightControlMessage.h"
#include "MQTTLightStatusMessage.h"

using namespace std;


class Coordinator : public IWifiStationListener, public IMqttClientListener{
private:
    WifiStation *m_wifistation;
    MqttClient *m_mqttclient;
    // MQTTMessage *m_mqttmessage;
    MQTTLightControlMessage *m_mqttmessage;
    MQTTLightStatusMessage *m_mqttmessage1;

public:
    Coordinator();
    virtual ~Coordinator();

    bool initialize();
    bool run();

    virtual void onNetworkConnected() override;
    virtual void onNetworkDisconnected() override;
    
    virtual void onReceiveTopic(string &topic, string &msg) override;
};


