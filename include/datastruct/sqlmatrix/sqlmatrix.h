#ifndef SQLXEIGEN_SQLMATRIX_H
#define SQLXEIGEN_SQLMATRIX_H

#include "datastruct/matrix/imatrix.h"

namespace sqlxeigen {
static const std::string INVALID_STRING = "-1";

template<typename M>
struct IRowView;

class SqlMatrix : public IMatrix, public std::enable_shared_from_this<SqlMatrix> {
private:
    struct LookupTable {
    private:
        std::vector<std::string> _indexToName;
        std::unordered_map<std::string, size_t> _nameToIndex;
    public:
        LookupTable(const std::vector<std::string>& names);

        const std::string& nameAt(size_t index) const;

        size_t indexOf(const std::string& name) const;

        const std::string& nameAt_Secure(size_t index) const;

        int indexOf_Secure(const std::string& name) const;

        int setName(size_t index, const std::string& name);

        void resize(size_t size);
    };


private:
    LookupTable _colNames;
    std::vector<mysqlx::Type> _colTypes;

public:
    SqlMatrix(const std::vector<mysqlx::Type>& colTypes = {}, const std::vector<std::string>& colNames = {}, size_t size = 0);
    ~SqlMatrix() = default;

    // Changing required functions accessor rights
    using IMatrix::get;
    using IMatrix::column;
    using IMatrix::columnRaw;


    template<typename T>
    inline T& get(size_t rowIndex, const std::string& colName);

    template<typename T>
    std::shared_ptr<Column<T>> column(const std::string& name);
    
    // initSize is only used if its the first column added to an empty matrix !
    // else current matrix.collength is used
    template<mysqlx::Type sqlT>
    void addColumn(const std::string& name, size_t initSize = 0);

    template<mysqlx::Type sqlT>
    void addColumn(const std::string& name, const std::vector<utils::type::sqlToRealType<sqlT>>& colData);

    std::shared_ptr<IColumn> columnRaw(const std::string& name);

    mysqlx::Type columnSqlType(size_t index) const;
    mysqlx::Type columnSqlType(const std::string& name) const;

    std::string columnName(size_t index) const;

    int renameColumn(size_t index, const std::string colName);

    using RowView = IRowView<SqlMatrix>;
    RowView rowView(size_t index);          // Defined in irowview.cpp
};




template<typename T>
inline T& SqlMatrix::get(size_t rowIndex, const std::string& colName) {
    return this->template get<T>(rowIndex, _colNames.indexOf(colName));
}

template<typename T>
std::shared_ptr<Column<T>> SqlMatrix::column(const std::string& name) {
    return this->template column<T>(_colNames.indexOf(name));
}

// initSize is only used if its the first column added to an empty matrix !
// else current matrix.collength is used
template<mysqlx::Type sqlT>
void SqlMatrix::addColumn(const std::string& name, size_t initSize) {
    IMatrix::template addColumn<utils::type::sqlToRealType<sqlT>>(initSize);
    _colNames.setName(cols() - 1, name);
    _colTypes.push_back(sqlT);
}

template<mysqlx::Type sqlT>
void SqlMatrix::addColumn(const std::string& name, const std::vector<utils::type::sqlToRealType<sqlT>>& colData) {
    IMatrix::template addColumn<utils::type::sqlToRealType<sqlT>>(colData);
    _colNames.setName(cols() - 1, name);
    _colTypes.push_back(sqlT);
}

};
#endif