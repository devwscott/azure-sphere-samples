#pragma once

#include <pthread.h>
#include <string>

#include "BitReadWriter.h"

using namespace std;


class Descriptor{
public:
    Descriptor();
    Descriptor(BitReadWriter* rw);
    virtual ~Descriptor();

    int GetDescriptorTag();
    int GetDescriptorLength();

    virtual void PrintDescriptor();
    virtual void PrintDescriptor(string descriptor_name);

    virtual void WriteDescriptor(BitReadWriter* rw);

protected:
    int descriptor_tag;
    int descriptor_length;

    virtual void updateDescriptorLength() = 0;
};


