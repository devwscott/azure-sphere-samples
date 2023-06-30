
#include <unistd.h>

#include <applibs/log.h>

#include "BitReadWriter.h"



BitReadWriter::BitReadWriter(){
    Log_Debug("BitReadWriter::BitReadWriter()\n");
    m_szBuf = NULL;
    m_nPos = 0;
    m_nOutCnt = 8;
    m_byBuffer = 0;
}


BitReadWriter::~BitReadWriter(){
    Log_Debug("BitReadWriter::~BitReadWriter()\n");

}


void BitReadWriter::SetBuffer(unsigned char* buffer){
    Log_Debug("BitReadWriter::SetBuffer()\n");
    m_szBuf = buffer;
    m_nPos = 0;
    m_nOutCnt = 8;
    m_byBuffer = 0;
}


int BitReadWriter::Read_On_Buffer(int nLen){
    Log_Debug("BitReadWriter::Read_On_Buffer()\n");

    unsigned int res = 0;
    unsigned int mask;
    unsigned char *sp = &m_szBuf[m_nPos];

    mask = 1 << (m_nOutCnt - 1);
    m_byBuffer = 0;

    for(int i=0; i<nLen; i++){
        m_byBuffer <<= 1;

        if(*sp & mask){
            m_byBuffer |= 1;
        }

        // Log_Debug("RD [%d] mask(0x%02x), Buffer(0x%02x)\n", i, mask, m_byBuffer);
        
        mask >>= 1;
        m_nOutCnt--;

        if(m_nOutCnt ==0){
            m_nOutCnt = 8;
            mask = 128; //b1000_0000
            m_nPos++;
            sp = &m_szBuf[m_nPos];  // sp++;
        }
    }

    res = m_byBuffer;

    return res;
}


void BitReadWriter::Skip_On_Buffer(int nLen){
    Log_Debug("BitReadWriter::Skip_On_Buffer()\n");

    for(int i=0; i<nLen; i++){
        m_nOutCnt--;

        if(m_nOutCnt == 0){
            m_nOutCnt = 8;
            m_nPos++;
        }
    }
}


void BitReadWriter::Write_On_Buffer(int value, int nLen){
    Log_Debug("BitReadWriter::Write_On_Buffer()\n");
    unsigned int mask;
    mask = 1 << (nLen-1);

    for(int i=0; i<nLen; i++){
        m_byBuffer <<= 1;

        if(value & mask){
            m_byBuffer |= 1;
        }
        
        // Log_Debug("WB [%d] mask(0x%02x), Buffer(0x%02x)\n", i, mask, m_byBuffer);

        mask >>= 1;
        m_nOutCnt--;

        if(m_nOutCnt == 0){
            m_szBuf[m_nPos] = m_byBuffer;
            m_nOutCnt = 8;
            m_nPos++;
            m_byBuffer = 0;
        }
    }
}


unsigned char* BitReadWriter::GetCurrentBuffer(){
    Log_Debug("BitReadWriter::GetCurrentBuffer()\n");

    return m_szBuf + m_nPos;
}


unsigned char* BitReadWriter::GetBufAdd(){
    Log_Debug("BitReadWriter::GetBufAdd()\n");

    return m_szBuf;
}


unsigned long BitReadWriter::calculate_crc32(unsigned char* temp, int nRd){
    Log_Debug("BitReadWriter::calculate_crc32()\n");

    // TODO : implements crc32()
}


