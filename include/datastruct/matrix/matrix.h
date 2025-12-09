#ifndef SQLXEIGEN_MATRIX_H
#define SQLXEIGEN_MATRIX_H

#include <string>
#include <vector>
#include "datastruct/matrix/imatrix.h"


namespace sqlxeigen {
template<typename M>
struct IRowView;

class Matrix : public IMatrix, public std::enable_shared_from_this<Matrix> {
public:
    Matrix(const std::vector<Type>& colTypes = {}, size_t size = 0);
    ~Matrix() = default;

    // initSize is only used if its the first column added to an empty matrix !
    // else current size is used
    using IMatrix::addColumn;
    using IMatrix::eraseColumn;
    using IMatrix::swapColumnPositions;

    using RowView = IRowView<Matrix>;
    RowView rowView(size_t index);          // Defined in irowview.cpp
};

};
#endif