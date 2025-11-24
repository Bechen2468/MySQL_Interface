#ifndef MYSQLX_BASEVIEW_H
#define MYSQLX_BASEVIEW_H

#include <iostream>
#include <string>
#include "mysql/connection_pool.h"
#include "datatype/datetime.h"
#include "datastruct/matrix.h"
#include "datastruct/row.h"



namespace sqlxeigen::view {

class BaseView {
public:
    matrix::Matrix result;
    
protected:

    const std::string _databaseName;

public:
    BaseView(const std::string& databaseName);

public: // mysql

protected:
    void _rebuildMatrix(const mysqlx::Columns& columns, size_t size);

    void _setFromSQLRow(mysqlx::Row& row, size_t index);

    std::string _escapeString(const std::string& str);
};

};
#endif