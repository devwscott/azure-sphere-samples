
#include "Blink.h"

#include <applibs/log.h>
#include <hw/template_appliance.h>
#include <cstdio>
#include <new>


int main(int argc, char *argv[])
{
    // Blink blink(TEMPLATE_LED);
    Blink *blink = new Blink(TEMPLATE_LED, "green");
    // blink.start(500);
    blink->start(4000);

    Log_Debug("Blink CPP Example start!!\n");

    Log_Debug("Color is %s\n", blink->getColor().c_str());

    while(1){
        //
    }

    return 0;
}