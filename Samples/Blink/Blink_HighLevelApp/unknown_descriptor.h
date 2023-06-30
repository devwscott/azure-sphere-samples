#pragma once

#include <pthread.h>
#include <string>

#include "descriptor.h"

using namespace std;


class UnknownDescriptor : public Descriptor{
public:
    UnknownDescriptor();
    UnknownDescriptor(BitReadWriter* rw);
    virtual ~UnknownDescriptor();

protected:
    virtual void updateDescriptorLength() override {};

};

