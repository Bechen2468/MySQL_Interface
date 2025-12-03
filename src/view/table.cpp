#include "view/table.h"

namespace sqlxeigen::view {


Table::Table(const std::string& databaseName, const std::string& tableName):
BaseView(databaseName),
_tableName(tableName)
{
}


bool Table::executeSelect(const std::vector<std::string>& columns, const std::string& where, const std::string& groupBy, const std::string& having, size_t limit) {
    try {
        std::shared_ptr<ConnectionPool> conPool = ConnectionPool::GetPool(_databaseName);
        std::unique_ptr<mysqlx::Session> session = conPool->get_session();
        
        mysqlx::Schema schema = session->getSchema(_databaseName);

        auto query = schema.getTable(_tableName).select(columns);
        if(where.length())   query.where(where);
        if(groupBy.length()) query.groupBy(groupBy);
        if(having.length())  query.having(having);
        if(limit != -1)      query.limit(limit);

        mysqlx::RowResult res = query.execute();
        mysqlx::Row row;// Permanently using fetchone over fetchall to overcome mem overhead for large sets


        // Resize
        this->_rebuildMatrix(res.getColumns(), res.count());

        // Set data
        for(int i = 0; row = res.fetchOne(); ++i) {
            this->_setFromSQLRow(row, i);
        }

        conPool->release_session(session);
    } catch(const std::exception &exc) {
        std::cerr << exc.what();
        return false;
    }

    return true;
}


int Table::executeInsert(sqlxeigen::matrix::Matrix& data, const std::unordered_map<std::string, std::string>& duplicateUpdate) {
    try {
        std::shared_ptr<ConnectionPool> conPool = ConnectionPool::GetPool(_databaseName);
        std::unique_ptr<mysqlx::Session> session = conPool->get_session();

        std::string query = this->_buildInsert(data, duplicateUpdate);
        auto sql = session->sql(query);

        int count = session->sql(query).execute().getAffectedItemsCount();

        conPool->release_session(session);

        return count;
    } catch(const std::exception &exc) {
        std::cerr << exc.what();
        return -1;
    }
}


int Table::executeDelete(const std::string& where) {
    try {
        std::shared_ptr<ConnectionPool> conPool = ConnectionPool::GetPool(_databaseName);
        std::unique_ptr<mysqlx::Session> session = conPool->get_session();

        mysqlx::Schema schema = session->getSchema(_databaseName);

        // Build query
        auto query = schema.getTable(_tableName).remove();
        if(where.length()) query.where(where);

        // Execute
        mysqlx::Result res = query.execute();
        int affected = res.getAffectedItemsCount();

        conPool->release_session(session);

        return affected;
    } catch(const std::exception &exc) {
        std::cerr << exc.what();
        return -1;
    }
}


std::string Table::_buildInsert(sqlxeigen::matrix::Matrix& data, const std::unordered_map<std::string, std::string>& duplicateUpdate) {
    std::string query = "INSERT INTO " + _tableName + " (";

    // Set fields
    for(size_t i = 0; i < data.cols(); ++i) {
        query += data.column(i).name() + (i != data.cols()-1 ? ", " : ") VALUES ");
    }

    // Add rows
    for(matrix::Row row = data.row(0); ; row = row.next()) {
        if(!row.isFirst()) query += ", ";
        query += "(";
        for(int i = 0; i < data.cols(); ++i) {
            if(i != 0) query += ",";
            
            // Insert data
            switch(data.column(i).type()) {
                case matrix::Column::Type::STRING:  query += _escapeString(row.get<std::string>(i));  break;
                case matrix::Column::Type::UINT8:   query += std::to_string(row.get<uint8_t>(i));      break;
                case matrix::Column::Type::INT:     query += std::to_string(row.get<int>(i));          break;
                case matrix::Column::Type::UINT64:  query += std::to_string(row.get<uint64_t>(i));     break;
                case matrix::Column::Type::FLOAT:   query += std::to_string(row.get<float>(i));        break;
                case matrix::Column::Type::DOUBLE:  query += std::to_string(row.get<double>(i));       break;
                case matrix::Column::Type::INT64:   query += std::to_string(row.get<int64_t>(i));    break;
            }
        }
        query += ")";

        if(row.isLast()) break;
    }

    // Set on duplicates
    if(duplicateUpdate.empty()) return query;

    query += " ON DUPLICATE KEY UPDATE ";

    for(auto keyV = duplicateUpdate.begin(); keyV != duplicateUpdate.end(); ++keyV) {
        if(keyV != duplicateUpdate.begin()) query += ", ";
        query += keyV->first + "=VALUES(" + keyV->second + ")";
    }

    return query;
}

};

