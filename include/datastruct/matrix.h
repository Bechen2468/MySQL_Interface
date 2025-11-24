#ifndef MY_MATRIX_H
#define MY_MATRIX_H

#include <string>
#include <vector>
#include "datatype/datetime.h"
#include "datastruct/column.h"


namespace sqlxeigen::matrix {
struct Row;

struct Matrix {

private:
    std::vector<Column> _columns;
    std::unordered_map<std::string, size_t> _nameToIndex;

public:
    Matrix() {};
    Matrix(const std::vector<mysqlx::Type>& colTypes, size_t size, const std::vector<std::string>& colNames = {});

    size_t rows() const;
    size_t cols() const;

    

    // Templating for inline access. !Only ment for small ramdom access:
    // For full range operations: Pull the vector itself with column()
    template<typename T>
    T& get(size_t row, const std::string& colName);

    template<typename T>
    T& get(size_t row, size_t col);
    

    // For full range operations:   Most efficient
    Column& column(const std::string& name);

    Column& column(size_t index);

    Row row(size_t index);  // Defined in row.cpp

    void addColumn(mysqlx::Type mysqlType, size_t size, const std::string& name = "");
};




template<typename T>
T& Matrix::get(size_t Row, const std::string& ColName) {
    auto col = _nameToIndex.find(ColName);
    if (col == _nameToIndex.end()) throw std::runtime_error("No such column: " + ColName);
    return (*this).template get<T>(Row, col->second);
}

template<typename T>
T& Matrix::get(size_t Row, size_t Col) {
    return _columns[Col].template get<T>(Row);
}




};

//#include "datastruct/row.h"
#endif
