
#include "Blink.h"

#include <applibs/log.h>
#include <hw/template_appliance.h>
#include <cstdio>
#include <new>

#include "Coordinator.h"

// added the __cxa_pure_virtual to support the virtual keyword of Interface
extern "C" void __cxa_pure_virtual() { while (1); }



int main(int argc, char *argv[])
{
    // Blink blink(TEMPLATE_LED);
    Blink *blink = new Blink(TEMPLATE_LED, "green");
    blink->start(4000);

    Coordinator *coordinator = new Coordinator();

    coordinator->initialize();
    // coordinator->run();

    // delete coordinator;

    Log_Debug("Blink CPP Example start!!\n");
    Log_Debug("Color is %s\n", blink->getColor().c_str());


    while(1){
        //
    }

    return 0;
}