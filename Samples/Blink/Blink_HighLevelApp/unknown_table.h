#pragma once

#include <pthread.h>
#include <string>

#include "table.h"

using namespace std;


class UnknownTable : public Table{
public:
    UnknownTable();
    UnknownTable(unsigned char* raw_buffer, unsigned int raw_length);
    virtual ~UnknownTable();

protected:
    virtual void __decode_table_body__() override;
};

