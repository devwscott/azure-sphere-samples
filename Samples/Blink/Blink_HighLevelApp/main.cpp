
#include "Blink.h"

#include <applibs/log.h>
#include <hw/template_appliance.h>
#include <cstdio>
#include <new>
#include <map>

#include "Coordinator.h"

// added the __cxa_pure_virtual to support the virtual keyword of Interface
extern "C" void __cxa_pure_virtual() { while (1); }



int main(int argc, char *argv[])
{
#if 0
    map<string, int> myMap;

    myMap["Alice"] = 25;
    myMap["Bob"] = 31;
    myMap["Charlie"] = 45;

    for(auto& pair:myMap){
        Log_Debug("%s:%d\n", pair.first.c_str(), pair.second);
    }

    for (auto it = myMap.begin(); it != myMap.end(); ++it) {
        Log_Debug("%s:%d\n", it->first.c_str(), it->second);
    }
    Log_Debug("--%d\n", myMap["Alice"]);
    Log_Debug("--%d\n", myMap["Bob"]);
    Log_Debug("--%d\n", myMap["Charlie"]);

    string test("hello_light");
    Log_Debug("-------- %s\n", test.c_str());

    if(test.find("hello") != string::npos){
        Log_Debug(">> find hello :  %s\n", test.c_str());
    }

    if(test.find("light") != string::npos){
        Log_Debug(">> find light :  %s\n", test.c_str());
    }

    if(test.find("curtain") != string::npos){
        Log_Debug(">> find curtain :  %s\n", test.c_str());
    }
    else
    {
        Log_Debug(">> Error : find curtain :  %s\n", test.c_str());
    }

#endif 



    // Blink blink(TEMPLATE_LED);
    Blink *blink = new Blink(TEMPLATE_LED, "green");
    blink->start(4000);

    Coordinator *coordinator = new Coordinator();

    coordinator->initialize();
    coordinator->run();

    // delete coordinator;

    Log_Debug("Blink CPP Example start!!\n");
    Log_Debug("Color is %s\n", blink->getColor().c_str());

    while(1){
        //
    }

    return 0;
}