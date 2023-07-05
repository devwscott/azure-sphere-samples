
#include <unistd.h>

#include <applibs/log.h>

#include "descriptor_factory.h"
#include "unknown_descriptor.h"
#include "light_control_descriptor.h"
#include "light_status_descriptor.h"



Descriptor* DescriptorFactory::CreateDescriptor(BitReadWriter* rw){
    Log_Debug("DescriptorFactory::CreateDescriptor(BitReadWriter* rw)\n");

    int descriptor_tag = (*(rw->GetCurrentBuffer()) & 0xFF);

    Log_Debug("[DescriptorFactory::CreateDescriptor] descriptor_tag = 0x%x\n", descriptor_tag);

    switch(descriptor_tag){
        case LIGHT_CONTROL_DESCRIPTOR:
            return new LightControlDescriptor(rw);
        
        case LIGHT_STATUS_DESCRIPTOR:
            return new LightStatusDescriptor(rw);
        
        default:
            return new UnknownDescriptor(rw);

    }
}


Descriptor* DescriptorFactory::CreateDescriptor(DescriptorTag tag){
    Log_Debug("DescriptorFactory::CreateDescriptor(DescriptorTag)\n");

    Log_Debug("[DescriptorFactory::CreateDescriptor] tag = 0x%x\n", tag);

    switch(tag){
        case LIGHT_CONTROL_DESCRIPTOR:
            return new LightControlDescriptor();
        
        case LIGHT_STATUS_DESCRIPTOR:
            return new LightStatusDescriptor();
        
        default:
            return new UnknownDescriptor();

    }
}


