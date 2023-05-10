#pragma once


#include "WifiStation.h" 

using namespace std;


class Coordinator : public IWifiStationListener{
private:
    WifiStation *m_wifistation;

public:
    Coordinator();
    virtual ~Coordinator();

    bool initialize();

    virtual void onNetworkConnected() override;
    virtual void onNetworkDisconnected() override;
};


