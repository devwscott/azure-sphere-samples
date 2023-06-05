
// It seems that the below includes can't be used in Azure Sphere
// #include <iostream>
// #include <stdexcept>
// #include <thread>

#include <unistd.h>

#include <applibs/log.h>

#include "WifiStation.h"
#include "Coordinator.h"


Coordinator::Coordinator(){
    Log_Debug("Coordinator::Coordinator()\n");

    string topic("test_topic");
    map<string, string> payload;
    
    payload["id"] = "1234";
    payload["state"] = "on";
    payload["key3"] = "value3";

    // string jsonstr("{\"id\":1234,\"control\":\"on\"}");
    //https://jsontostring.com/
    string jsonstr("{\"type\":\"Light\",\"control\":\"on\",\"desired\":{\"id\":1234,\"state\":\"on\"}}");

    // m_mqttmessage = new MQTTMessage(topic, payload);
    // m_mqttmessage = new MQTTMessage(topic, jsonstr);
    // m_mqttmessage = new MQTTLightControlMessage(topic, jsonstr);
    // m_mqttmessage1 = new MQTTLightStatusMessage(topic, payload);

    topic = "light_control";
    m_LightCodec = new MQTTMessageLightCodec(topic);
    m_mqttmessage = m_LightCodec->decode(jsonstr);

    topic = "light_status";
    m_LightCodec = new MQTTMessageLightCodec(topic);
    m_mqttmessage1 = m_LightCodec->encode(payload);

    // m_wifistation = new WifiStation(this);
    // if(m_wifistation == NULL)
    // {
    //     Log_Debug("new WifiSTation() error\n");
    // }

    // m_mqttclient = new MqttClient(this);
    // if(m_mqttclient == NULL)
    // {
    //     Log_Debug("new MqttClient() error\n");
    // }

}



Coordinator::~Coordinator(){
    Log_Debug("Coordinator::~Coordinator()\n");

    delete m_wifistation;
    delete m_mqttclient;
}


bool Coordinator::initialize(){
    bool ret = true;

    Log_Debug("Coordinator::initialize\n");

    string topic;
    map<string, string> payload;
    string jsonstr;


    Log_Debug("-------------MQTTLIGHT CONTROL-----------------\n");
    topic = m_mqttmessage->getTopic();
    payload = m_mqttmessage->getPayload();
    jsonstr = m_mqttmessage->toJson();
    // topic = ((MQTTLightControlMessage*)m_mqttmessage)->getType();
    // topic = dynamic_cast<MQTTLightControlMessage*>(m_mqttmessage)->getType();

    Log_Debug("Get Topic : %s\n", topic.c_str());

    for (auto it = payload.begin(); it != payload.end(); ++it) {
        Log_Debug("[Payload] %s:%s\n", it->first.c_str(), it->second.c_str());
    }

    Log_Debug("Get json : %s\n", jsonstr.c_str());

    Log_Debug("-------------MQTTLIGHT STATUS-----------------\n");

    topic = m_mqttmessage1->getTopic();
    payload = m_mqttmessage1->getPayload();
    jsonstr = m_mqttmessage1->toJson();

    Log_Debug("Get Topic : %s\n", topic.c_str());

    for (auto it = payload.begin(); it != payload.end(); ++it) {
        Log_Debug("[Payload] %s:%s\n", it->first.c_str(), it->second.c_str());
    }

    Log_Debug("Get json : %s\n", jsonstr.c_str());





#if 0
    if(m_wifistation->init("TestAP") != true){
    // if(m_wifistation->init("TestAP_LAB") != true){
        Log_Debug("[Coordinator::initialize] error m_wifistation->init()\n");
        return false;
    }

    Log_Debug("AP NAME after is %s\n", m_wifistation->TEST_getAPName().c_str());

    // QOS(0), keepAlive(60 sec)
    if(m_mqttclient->init(0, 60) != true){
        Log_Debug("[Coordinator::initialize] error : m_mqttclient->init()\n");
        return false;
    }
#endif 

    return ret;
}


bool Coordinator::run(){
    bool ret = true;

    Log_Debug("Coordinator::run\n");
    
    if(m_wifistation->connect() != true){
        // for testing : storeAPInfo()
        m_wifistation->storeAPInfo("SK_WiFiGIGA4E04_5G", WifiStation::Security_Wpa2Psk, "KMR24@7966");
        // m_wifistation->storeAPInfo("ZIGBANG", WifiStation::Security_Wpa2Psk, "wlrqkd0#");
    }


    return ret;
}


void Coordinator::onNetworkConnected(){
    bool bConnected;
    string ipAddress;
    string macAddress;

    Log_Debug("Coordinator::onNetworkConnected\n");

    m_wifistation->getInfo(bConnected, ipAddress, macAddress);

    Log_Debug("Status : %s\n", bConnected==true?"Connected":"DisConnected");
    Log_Debug("IP : %s\n", ipAddress.c_str());
    Log_Debug("MAC : %s\n", macAddress.c_str());

    // for testing, disConnect()
    // usleep(2000*1000);
    // m_wifistation->disConnect();

#if defined(MQTT_USE_MBEDTLS)
    if(m_mqttclient->connect(string("192.168.45.197"), string("8883"), "ca_cert.der") != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->connect() with CA Cert\n");
    }
#else 
    /* MQTT Connect without CA Cert */
    if(m_mqttclient->connect(string("192.168.45.197"), string("1883")) != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->connect() without CA Cert\n");
    }
#endif 

    /* For Testing, publish MQTT */
    usleep(1000*1000);
    string test_topic = "hello";
    string test_msg = "mqttclient_msg";
    
    if(m_mqttclient->publish(test_topic, test_msg) != true){
        Log_Debug("[Coordinator::onNetworkConnected] error : m_mqttclient->publish()\n");
    }
    
    return;
}



void Coordinator::onNetworkDisconnected(){
    bool bConnected;
    string ipAddress;
    string macAddress;

    Log_Debug("Coordinator::onNetworkDisconnected\n");

    m_wifistation->getInfo(bConnected, ipAddress, macAddress);

    Log_Debug("Status : %s\n", bConnected==true?"Connected":"DisConnected");
    Log_Debug("IP : %s\n", ipAddress.c_str());
    Log_Debug("MAC : %s\n", macAddress.c_str());


    // for testing, connect()
    usleep(2000*1000);
    m_wifistation->connect();

    return;
}



void Coordinator::onReceiveTopic(string &topic, string &msg){
    Log_Debug("Coordinator::onReceiveTopic\n");

    Log_Debug("     topic:%s\n", topic.c_str());
    Log_Debug("     msg:%s\n", msg.c_str());

    return;
}


