#ifndef SQLXEIGEN_TABLE_VIEW_H
#define SQLXEIGEN_TABLE_VIEW_H

#include <string>
#include "view/baseview.h"



namespace sqlxeigen {

class TableView : public BaseView {
private:
    std::string _tableName;

public:
    TableView(const std::string& databaseName, const std::string& tableName);

    // Using previously set internal conditions if left empty
    bool executeSelect(const std::vector<std::string>& columns = {"*"}, const std::string& where = "", const std::string& groupBy = "", const std::string& having = "", size_t limit = -1);
    int executeInsert(std::shared_ptr<SqlMatrix>& data, const std::unordered_map<std::string, std::string>& duplicateUpdate = {});
    int executeDelete(const std::string& where = "");

private:
    std::string _buildInsert(std::shared_ptr<SqlMatrix>& data, const std::unordered_map<std::string, std::string>& duplicateUpdate = {});
};

};
#endif
