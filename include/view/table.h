#ifndef TABLE_VIEW_H
#define TABLE_VIEW_H

#include <string>
#include <mysqlx/xdevapi.h>
#include "datastruct/matrix.h"
#include "view/baseview.h"



namespace sqlxeigen::view {

class Table : public BaseView {
private:
    std::string _tableName;

public:
    Table(const std::string& databaseName, const std::string& tableName);

    // Using previously set internal conditions if left empty
    bool executeSelect(const std::vector<std::string>& columns = {"*"}, const std::string& where = "", const std::string& groupBy = "", const std::string& having = "", size_t limit = -1);
    int executeInsert(sqlxeigen::matrix::Matrix& data, const std::unordered_map<std::string, std::string>& duplicateUpdate = {});
    int executeDelete(const std::string& where = "");

private:
    std::string _buildInsert(sqlxeigen::matrix::Matrix& data, const std::unordered_map<std::string, std::string>& duplicateUpdate = {});
};

};
#endif
