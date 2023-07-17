#pragma once

#include <pthread.h>
#include <string>

#include "descriptor.h"
#include "table.h"

using namespace std;


class LightTable : public Table{
public:
    LightTable();
    LightTable(unsigned char* raw_buffer);
    LightTable(unsigned char* raw_buffer, unsigned int raw_length);
    virtual ~LightTable();

    virtual void PrintTable() override;
    int GetVersionNumber();
    void SetVersionNumber(int version);

    int GetCoreId();
    void SetCoreId(int id);

    void SetDescriptor(Descriptor* desc);

protected:
    int core_id;
    unsigned char version_number;
    Descriptor* descriptor;

    virtual void __decode_table_body__() override;
    virtual void __encode_update_table_length__() override;
    virtual void __encode_prepare_table__() override;
    virtual void __encode_write_table_body__() override;

};

