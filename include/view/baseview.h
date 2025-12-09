#ifndef SQLXEIGEN_BASEVIEW_H
#define SQLXEIGEN_BASEVIEW_H

#include <iostream>
#include <string>
#include "mysql/connection_pool.h"
#include "datastruct/irowview.h"



namespace sqlxeigen {

class BaseView {
public:
    std::shared_ptr<SqlMatrix> result;

protected:
    const std::string _databaseName;

public:
    BaseView(const std::string& databaseName);

protected:
    void _rebuildMatrix(const mysqlx::Columns& columns, size_t size);

    void _setFromSQLRow(mysqlx::Row& row, size_t index);

    std::string _escapeString(const std::string& str);
};

};
#endif