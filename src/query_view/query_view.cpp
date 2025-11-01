#include <query_view/query_view.h>



QueryView::QueryView(const std::string& Query, const std::string& Database):
_querySelect(Query),
_database(Database)
{

}



size_t QueryView::rows() const {
    return _columns[0].size();
}



size_t QueryView::cols() const {
    return _columns.size();
}



template<typename T>
T& QueryView::operator()(size_t Row, const std::string& ColName) {
    auto col = _nameToIndex.find(ColName);
    if (col == _nameToIndex.end()) throw std::runtime_error("No such column: " + colName);
    return (*this)(Row, col->second);
}



template<typename T>
T& QueryView::operator()(size_t Row, size_t Col) {
    return _columns[Col].operator()<T>(Row);
}



template<typename VecT>
VecT& QueryView::column(const std::string& Name) {
    auto col = _nameToIndex.find(Name);
    if (col == _nameToIndex.end()) throw std::runtime_error("No such column: " + Name);

    return (*this).column<VecT>(col->second);
}



template<typename VecT>
VecT& QueryView::column(size_t Index) {
    return *(_columns[Index].get_column<VecT>());
}




void QueryView::setWhere(std::string Where) {
    _querySelectWhere = Where;
}


void QueryView::setOrderBy(std::string Order) {
    _querySelectOrderBy = Order;
}


void QueryView::setLimit(int Limit) {
    _querySelectLimit = Limit;
}


void  QueryView::update() {
    std::shared_ptr<ConnectionPool> conPool = GetConnectionPool(_database);
    std::unique_ptr<mysqlx::Session> session = conPool->get_session();
    
}



std::string  QueryView::_buildQuery() {
    std::string query = _querySelect;
    if(_querySelectWhere.length()) query += "WHERE(" + _querySelectWhere + ")";
    if(_querySelectOrderBy.length()) query += "ORDER BY " + _querySelectOrderBy;
    if(_querySelectLimit > 0) query += "LIMIT " + std::to_string(_querySelectLimit);
}

