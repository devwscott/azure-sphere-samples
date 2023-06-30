
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


/* TEST Codes which shoule be located in main.cpp

    unsigned char buff_data[] = {0x47, 0x01, 0xb0, 0x0c, 0x52, 0x54, 0xd5, 0xff, 0xff, 0x00, 0x01, 0x1f, 0x12, 0x34, 0x56, 0x78};
    int value;

    unsigned char* buff;
    buff = new unsigned char[20];
    
    memcpy(buff, buff_data, sizeof(buff_data));

    BitReadWriter *bitRW = new BitReadWriter();
    bitRW->SetBuffer(buff);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("magic : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("table_id : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(1);
    Log_Debug("section_syntax : 0x%x\n", value);

    bitRW->Skip_On_Buffer(3);

    value = bitRW->Read_On_Buffer(12);
    Log_Debug("section_length : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(16);
    Log_Debug("coreid : 0x%x\n", value);
    Log_Debug("coreid : %c%c\n", (value&0xFF00)>>8, value&0xFF);

    bitRW->Skip_On_Buffer(2);

    value = bitRW->Read_On_Buffer(5);
    Log_Debug("version : 0x%x\n", value);

    bitRW->Skip_On_Buffer(17);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("desc.tag : 0x%x\n", value);

    value = bitRW->Read_On_Buffer(8);
    Log_Debug("desc.length : 0x%x\n", value);


    value = bitRW->Read_On_Buffer(4);
    Log_Debug("device id : 0x%x\n", value);


    bitRW->Skip_On_Buffer(3);

    value = bitRW->Read_On_Buffer(1);
    Log_Debug("device status : 0x%x\n", value);


    value = bitRW->Read_On_Buffer(32);
    Log_Debug("CRC_32 : 0x%x\n", value);

    unsigned char* buff2;
    buff2 = bitRW->GetBufAdd();

    for(int i=0; i<16; i++){
        Log_Debug("[%d] 0x%02x\n", i, buff2[i]);
    }



#if 0//write test
    bitRW->Write_On_Buffer(0x47, 8);
    bitRW->Write_On_Buffer(0x01, 8);
    bitRW->Write_On_Buffer(0x1, 1);
    bitRW->Write_On_Buffer(0x0, 1);
    bitRW->Write_On_Buffer(0x3, 2);
    bitRW->Write_On_Buffer(0x0C, 12);
    bitRW->Write_On_Buffer(0x52, 8);
    bitRW->Write_On_Buffer(0x54, 8);
    bitRW->Write_On_Buffer(0x3, 2);
    bitRW->Write_On_Buffer(0xA, 5);
    bitRW->Write_On_Buffer(0x1, 1);
    bitRW->Write_On_Buffer(0xFF, 8);
    bitRW->Write_On_Buffer(0xFF, 8);
    bitRW->Write_On_Buffer(0x00, 8);
    bitRW->Write_On_Buffer(0x01, 8);
    bitRW->Write_On_Buffer(0x01, 4);
    bitRW->Write_On_Buffer(0x0F, 3);
    bitRW->Write_On_Buffer(0x01, 1);
    bitRW->Write_On_Buffer(0x12345678, 32);

    for(int i=0; i<15; i++){
        Log_Debug("[%d] 0x%02x\n", i, buff[i]);
    }
#endif

*/