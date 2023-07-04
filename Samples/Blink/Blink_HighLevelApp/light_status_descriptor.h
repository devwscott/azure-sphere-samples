#pragma once

#include <pthread.h>
#include <string>

#include "descriptor.h"

using namespace std;


class LightStatusDescriptor : public Descriptor{
public:
    LightStatusDescriptor();
    LightStatusDescriptor(BitReadWriter* rw);
    virtual ~LightStatusDescriptor();

    virtual void PrintDescriptor() override;
    virtual void WriteDescriptor(BitReadWriter* rw) override;

    int GetDeviceCount();
    int GetDeviceStatus(int deviceIndex);

    void SetDeviceStatus(int deviceId, int deviceStatus);

protected:
    virtual void updateDescriptorLength() override;

private:
    int m_deviceCount;
    int m_deviceId[16];
    int m_deviceStatus[16];

};


