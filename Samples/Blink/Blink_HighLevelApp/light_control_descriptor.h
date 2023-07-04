#pragma once

#include <pthread.h>
#include <string>

#include "descriptor.h"

using namespace std;


class LightControlDescriptor : public Descriptor{
public:
    LightControlDescriptor();
    LightControlDescriptor(BitReadWriter* rw);
    virtual ~LightControlDescriptor();

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


