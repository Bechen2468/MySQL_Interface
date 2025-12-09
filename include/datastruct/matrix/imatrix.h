#ifndef SQLXEIGEN_IMATRIX_H
#define SQLXEIGEN_IMATRIX_H

#include <string>
#include <vector>
#include "datastruct/matrix/column.h"


namespace sqlxeigen {
class IMatrix {
protected:
    std::vector<std::shared_ptr<IColumn>> _columns;

public:
    IMatrix(const std::vector<Type>& colTypes = {}, size_t size = 0);
    virtual ~IMatrix();

    size_t rows() const;
    size_t cols() const;

    Type columnType(size_t colIndex) const;

    void resize(size_t size);
    void reserve(size_t resSize);


    template<typename T>
    inline T& get(size_t rowIndex, size_t colIndex);


    template<typename T>
    std::shared_ptr<Column<T>> column(size_t index);

    std::shared_ptr<IColumn> columnRaw(size_t index);


protected:
    // initSize is only used if its the first column added to an empty matrix !
    // else current matrix.collength is used
    template<typename T>
    void addColumn(size_t initSize = 0);

    template<Type T>
    void addColumnByType(size_t initSize = 0);

    template<typename T>
    void addColumn(const std::vector<T>& colData);

    // Just realeases the column without deletion (is shared_ptr)
    void eraseColumn(size_t index);

    void swapColumnPositions(size_t i, size_t j);
};




template<typename T>
inline T& IMatrix::get(size_t rowIndex, size_t colIndex) {
    return static_cast<Column<T>*>(_columns[colIndex].get())->get(rowIndex);
}


template<typename T>
std::shared_ptr<Column<T>> IMatrix::column(size_t index) {
    return std::static_pointer_cast<Column<T>>(_columns[index]);
}


template<typename T>
void IMatrix::addColumn(size_t initSize) {
    _columns.push_back(std::make_shared<Column<T>>(cols() == 0 ? initSize : rows()));
}


template<Type T>
void IMatrix::addColumnByType(size_t initSize) {
    using realType = utils::type::toRealType<T>;
    addColumn<realType>(cols() == 0 ? initSize : rows());
}


template<typename T>
void IMatrix::addColumn(const std::vector<T>& colData) {
    _columns.push_back(std::make_shared<Column<T>>(colData));
}

};
#endif
