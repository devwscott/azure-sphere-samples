#pragma once

#include <pthread.h>
#include <string>

using namespace std;

class BitReadWriter{
public:
    BitReadWriter();
    virtual ~BitReadWriter();

    unsigned char* GetCurrentBuffer();
    unsigned char* GetBufAdd(); //GetBufferAddress?
    
    void SetBuffer(unsigned char* buffer);
    int Read_On_Buffer(int nLen);
    void Write_On_Buffer(int value, int nLen);
    void Skip_On_Buffer(int nLen);

    unsigned long calculate_crc32(unsigned char* temp, int nRd);

private:
    unsigned char* m_szBuf;
    int m_nPos;     // m_nBytePosition?
    int m_nOutCnt;  // m_nBitPosition?
    unsigned int m_byBuffer;    //m_OutputByte?
};


