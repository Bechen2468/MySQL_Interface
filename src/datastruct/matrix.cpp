#include "datastruct/matrix.h"

namespace sqlxeigen::matrix {

Matrix::Matrix(const std::vector<mysqlx::Type>& colTypes, size_t size, const std::vector<std::string>& colNames) {
    for(int i = 0; i < colTypes.size(); ++i) {
        addColumn(
            colTypes[i],
            size,
            colNames.size() <= i ? "" : colNames[i] 
        );
    }
}


size_t Matrix::rows() const {
    return _columns.empty() ? 0 : _columns[0].size();
}



size_t Matrix::cols() const {
    return _columns.size();
}



Column& Matrix::column(const std::string& Name) {
    auto col = _nameToIndex.find(Name);
    if (col == _nameToIndex.end()) throw std::runtime_error("No such column: " + Name);

    return this->column(col->second);
}



Column& Matrix::column(size_t Index) {
    return _columns[Index];
}



void Matrix::addColumn(mysqlx::Type mysqlType, size_t size, const std::string& name) {
    _columns.push_back(Column(mysqlType, size, name));
    _nameToIndex.insert_or_assign(_columns.back().name(), cols()-1);
}


};