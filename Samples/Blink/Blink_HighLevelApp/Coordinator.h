#pragma once


#include "WifiStation.h" 
#include "MqttClient.h" 

using namespace std;


class Coordinator : public IWifiStationListener, public IMqttClientListener{
private:
    WifiStation *m_wifistation;
    MqttClient *m_mqttclient;

public:
    Coordinator();
    virtual ~Coordinator();

    bool initialize();
    bool run();

    virtual void onNetworkConnected() override;
    virtual void onNetworkDisconnected() override;
    
    virtual void onReceiveTopic(string &topic, string &msg) override;
};


