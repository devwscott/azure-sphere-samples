#pragma once

#include <pthread.h>
#include <string>

using namespace std;


class IWifiStationListener{
public:
    virtual void onNetworkConnected() = 0;
    virtual void onNetworkDisconnected() = 0;
};


class WifiStation{
private:
    string m_apName;
    bool m_bConnected;
    pthread_t m_thread;
    bool m_running;
    IWifiStationListener *m_listener;

    static void* __statusThread__(void* arg);

public:
    WifiStation(IWifiStationListener *callback);
    virtual ~WifiStation();

    bool init(string apName);
    bool connect();
    void disConnect();

    typedef enum{
        Security_Unknown = 0,
        Security_Open,
        Security_Wpa2Psk,
        Security_Reserved
    }SecurityType;

    bool storeAPInfo(string ssid, SecurityType secType, string passwd);
    void getInfo(bool &bConnected, string &ipAddr, string &macAddr);

    string TEST_getAPName();
};

