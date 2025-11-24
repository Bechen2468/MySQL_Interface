#include "view/query.h"


namespace sqlxeigen {

view::Query::Query(const std::string& Querystr, const std::string& Database):
BaseView(Database),
_querySelect(Querystr)
{
}




void view::Query::setWhere(const std::string& Where) {
    _querySelectWhere = Where;
}


void view::Query::setOrderBy(const std::string& Order) {
    _querySelectOrderBy = Order;
}


void view::Query::setLimit(int Limit) {
    _querySelectLimit = Limit;
}


void view::Query::executeSelect() {
    std::shared_ptr<ConnectionPool> conPool = ConnectionPool::GetPool(_databaseName);
    std::unique_ptr<mysqlx::Session> session = conPool->get_session();
    
    mysqlx::SqlStatement statement = session->sql(this->_buildQuery());
    mysqlx::SqlResult res = statement.execute();
    mysqlx::Row row;// Permanently using fetchone over fetchall to overcome mem overhead for large sets


    // Resize
    this->_rebuildMatrix(res.getColumns(), res.count());

    // Set data
    for(int i = 0; row = res.fetchOne(); ++i) {
        this->_setFromSQLRow(row, i);
    }

    conPool->release_session(session);
}


std::string view::Query::_buildQuery() {
    std::string query = _querySelect;
    if(_querySelectWhere.length())      query += " WHERE(" + _querySelectWhere + ")";
    if(_querySelectOrderBy.length())    query += " ORDER BY " + _querySelectOrderBy;
    if(_querySelectLimit > 0)           query += " LIMIT " + std::to_string(_querySelectLimit);
    return query;
}

};