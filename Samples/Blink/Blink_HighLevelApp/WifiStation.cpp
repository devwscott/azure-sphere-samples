
#include <unistd.h>

#include <applibs/log.h>
#include <applibs/wificonfig.h>
#include <applibs/networking.h>

#include <ifaddrs.h>
#include <net/ethernet.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <arpa/inet.h>

#include "WifiStation.h"


WifiStation::WifiStation(IWifiStationListener *callback):m_apName(""), m_bConnected(false), m_running(false){
    Log_Debug("WifiStation::WifiStation()\n");
    m_listener = callback;
}



WifiStation::~WifiStation(){
    Log_Debug("WifiStation::~WifiStation()\n");

    if(pthread_cancel(m_thread) != 0){
        Log_Debug("[WifiStation::~WifiStation] error : pthread_cancel()\n");
    }

}



bool WifiStation::init(string apName){
    bool ret = true;

    Log_Debug("WifiStation::init()\n");
    m_apName = apName;

    if(!m_apName.empty()){
        int networkId;
        networkId = WifiConfig_GetNetworkIdByConfigName(m_apName.c_str());
        if(networkId != -1)
        {
            if(WifiConfig_SetNetworkEnabled(networkId, false) == -1){
                Log_Debug("[WifiStation::init] error : set Network Enabled(false)\n");
                return false;
            }
        }
        else{
            Log_Debug("[WifiStation::init] Invalid networkId\n");
        }

        if(pthread_create(&m_thread, NULL, WifiStation::__statusThread__, this) != 0){
            Log_Debug("[WifiStation::init] error : pthread_create()\n");
            ret = false;
        }

        m_running = true;

    }else{
        Log_Debug("[WifiStation::init] apName is empty\n");
        ret = false;
    }

    return ret;
}


bool WifiStation::connect(){
    bool ret = true;
    
    Log_Debug("WifiStation::connect()\n");

    int networkId;
    networkId = WifiConfig_GetNetworkIdByConfigName(m_apName.c_str());
    if(networkId != -1)
    {
        if(WifiConfig_SetNetworkEnabled(networkId, true) == -1){
            Log_Debug("[WifiStation::connect] error : set Network Enabled(true)\n");
            ret = false;
        }
    }
    else
    {
        Log_Debug("[WifiStation::connect] Invalid networkId\n");
        ret = false;
    }

    return ret;
}



void WifiStation::disConnect(){
    Log_Debug("WifiStation::disConnect()\n");
    
    int networkId;
    networkId = WifiConfig_GetNetworkIdByConfigName(m_apName.c_str());
    if(networkId != -1)
    {
        if(WifiConfig_SetNetworkEnabled(networkId, false) == -1){
            Log_Debug("[WifiStation::disConnect] error : set Network Enabled(false)\n");
            return ;
        }
    }
    else{
        Log_Debug("[WifiStation::disConnect] Invalid networkId\n");
    }

    return;
}


bool WifiStation::storeAPInfo(string ssid, SecurityType secType, string passwd){
    bool ret = true;

    Log_Debug("WifiStation::storeAPInfo()\n");

    if(ssid.empty() || (secType!=Security_Wpa2Psk) || passwd.empty())
    {
        Log_Debug("[WifiStation::storeAPInfo] error : invalid param\n");
        return false;
    }

    if(WifiConfig_ForgetAllNetworks() == -1){
        Log_Debug("[WifiStation::storeAPInfo] error : WifiConfig_ForgetAllNetworks()\n");
        return false;
    }

    int networkId;
    networkId = WifiConfig_AddNetwork();
    if(networkId != -1){
        
        if(WifiConfig_SetSecurityType(networkId, WifiConfig_Security_Wpa2_Psk) == -1){
            Log_Debug("[WifiStation::storeAPInfo] error : WifiConfig_SetSecurityType()\n");
            return false;
        }

        if(WifiConfig_SetSSID(networkId, (const unsigned char*)ssid.c_str(), ssid.length()) == -1){
            Log_Debug("[WifiStation::storeAPInfo] error : WifiConfig_SetSSID()\n");
            return false;
        }

        if(WifiConfig_SetPSK(networkId, passwd.c_str(), passwd.length()) == -1){
            Log_Debug("[WifiStation::storeAPInfo] error : WifiConfig_SetPSK()\n");
            return false;
        }

        if(WifiConfig_SetConfigName(networkId, m_apName.c_str()) == -1){
            Log_Debug("[WifiStation::storeAPInfo] error : WifiConfig_SetConfigName()\n");
            return false;
        }

        if(WifiConfig_PersistConfig() == -1){
            Log_Debug("[WifiStation::storeAPInfo] error : WifiConfig_PersistConfig()\n");
            return false;
        }
    }
    else
    {
        ret = false;
    }

    return ret;
}



void WifiStation::getInfo(bool &bConnected, string &ipAddr, string &macAddr){
    Log_Debug("WifiStation::getInfo()\n");
    
    struct ifaddrs* addr_list;
    struct ifaddrs* it;
    int n;
    const char EthInterface[] = "wlan0";

    if(m_bConnected){
        if (getifaddrs(&addr_list) < 0) {
            Log_Debug("[WifiStation::getInfo] error : getifaddrs()\n");
        }
        else {
            for (it = addr_list, n = 0; it != NULL; it = it->ifa_next, ++n) {
                if (it->ifa_addr == NULL) {
                    continue;
                }
                if (strncmp(it->ifa_name, EthInterface, sizeof(EthInterface)) == 0) {
                    if (it->ifa_addr->sa_family == AF_INET) {
                        struct sockaddr_in* addr = (struct sockaddr_in*)it->ifa_addr;
                        char* ip_address = inet_ntoa(addr->sin_addr);
                        // Log_Debug("**** wlan0 IP found: %s ***\n", ip_address);
                        
                        ipAddr.append(ip_address);
                    }
                    else if (it->ifa_addr->sa_family == AF_PACKET) {
                        struct sockaddr_ll* mac_addr = (struct sockaddr_ll*)it->ifa_addr;
                        unsigned char* mac = mac_addr->sll_addr;

                        char mac_string[20];
                        sprintf(mac_string, "%02x:%02x:%02x:%02x:%02x:%02x", mac[0],
                            mac[1], mac[2], mac[3], mac[4], mac[5]);
                        // Log_Debug("**** wlan0 MAC found: %s ****\n", mac_string);

                        macAddr.append(mac_string);
                    }
                }
            }
        }
        freeifaddrs(addr_list);
    }

    bConnected = m_bConnected;

    return;
}



string WifiStation::TEST_getAPName(){
    return m_apName;
}



void* WifiStation::__statusThread__(void *arg){
    WifiStation *pWifiStation = static_cast<WifiStation*>(arg);

    static int cnt;
    int runInterval = 500*1000;     //500ms sleep()
    Networking_InterfaceConnectionStatus status;

    Log_Debug("WifiStation::__statusThread__()\n");

    while(1){

        if(pWifiStation->m_running){
            Log_Debug("[WifiStation::__statusThread__] running!(%d)\n", cnt++);

            if(Networking_GetInterfaceConnectionStatus("wlan0", &status) != -1)
            {
                Log_Debug("[WifiStation::__statusThread__] status : %d\n", status);
                if((status & Networking_InterfaceConnectionStatus_InterfaceUp) &&
                    (status & Networking_InterfaceConnectionStatus_ConnectedToNetwork) &&
                    (status & Networking_InterfaceConnectionStatus_IpAvailable))
                {
                    if(pWifiStation->m_bConnected == false){
                        pWifiStation->m_bConnected = true;
                        Log_Debug("[WifiStation::__statusThread__] WIFI Connected!!\n");
                        pWifiStation->m_listener->onNetworkConnected();
                    }
                }
                else
                {
                    if(pWifiStation->m_bConnected == true){
                        Log_Debug("[WifiStation::__statusThread__] WIFI Disconnected!!\n");
                        pWifiStation->m_bConnected = false;
                        pWifiStation->m_listener->onNetworkDisconnected();
                    }
                    
                }

            }
        }

        usleep(runInterval);
    }

    return nullptr;
}



