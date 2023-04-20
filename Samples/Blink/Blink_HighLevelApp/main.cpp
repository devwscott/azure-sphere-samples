
#include "Blink.h"

#include <applibs/log.h>
#include <hw/template_appliance.h>

int main(int argc, char *argv[])
{
    Blink blink(TEMPLATE_LED);
    // blink.start(500);
    blink.start(2000);

    Log_Debug("Blink CPP Example start!!\n");

    while(1){
        //
    }

    return 0;
}