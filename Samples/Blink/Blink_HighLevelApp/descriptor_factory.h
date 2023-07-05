#pragma once

#include <pthread.h>
#include <string>

#include "BitReadWriter.h"
#include "descriptor.h"

using namespace std;


class DescriptorFactory{
public:
    enum DescriptorTag{
        LIGHT_CONTROL_DESCRIPTOR = 0x00,
        LIGHT_STATUS_DESCRIPTOR = 0x01,
        LIGHT_FULL_CONTROL_DESCRIPTOR = 0x02,

        CURTAIN_CONTROL_DESCRIPTOR = 0x03,
        CURTAIN_STATUS_DESCRIPTOR = 0x04,

        SYSTEM_DISCOVERY_DESCRIPTOR = 0x10,
        
        UNKNOWN_DESCRIPTOR = 0xFF
    };

    static Descriptor* CreateDescriptor(BitReadWriter* rw);
    static Descriptor* CreateDescriptor(DescriptorTag tag);


private:
    DescriptorFactory(){};
    virtual ~DescriptorFactory(){};
};

