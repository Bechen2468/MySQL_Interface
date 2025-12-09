#ifndef SQLXEIGEN_ROWVIEW_H
#define SQLXEIGEN_ROWVIEW_H

#include "datastruct/matrix/matrix.h"
#include "datastruct/sqlmatrix/sqlmatrix.h"

namespace sqlxeigen {

template<typename M>
struct IRowView {
    std::shared_ptr<M> matrix;
    size_t index;

    IRowView(std::shared_ptr<M> matrix, size_t index);


    void operator=(IRowView<M> r);

    template<typename T>
    T& get(size_t colIndex);

    template<typename T>
    T& get(const std::string& colName) requires std::is_same_v<M, SqlMatrix>;

    IRowView<M> next() const;
    IRowView<M> previous() const;
    IRowView<M> nextSecure() const;
    IRowView<M> previousSecure() const;

    bool isFirst() const;
    bool isLast() const;
};


template<typename M>
IRowView<M>::IRowView(std::shared_ptr<M> matrix, size_t index):
matrix(matrix),
index(index)
{

}


template<typename M>
void IRowView<M>::operator=(IRowView<M> r) {
    this->matrix = r.matrix;
    this->index = r.index;
}

template<>
template<typename T>
T& IRowView<SqlMatrix>::get(const std::string& colName) requires std::is_same_v<SqlMatrix, SqlMatrix> {
    return matrix->template get<T>(index, colName);
}


template<typename M>
template<typename T>
T& IRowView<M>::get(size_t colIndex) {
    return matrix->template get<T>(index, colIndex);
}


template<typename M>
IRowView<M> IRowView<M>::next() const { return IRowView<M>(matrix, index + 1); }

template<typename M>
IRowView<M> IRowView<M>::previous() const { return IRowView<M>(matrix, index - 1); }

template<typename M>
IRowView<M> IRowView<M>::nextSecure() const { return IRowView<M>(matrix, std::min(index + 1, matrix->rows() - 1)); }

template<typename M>
IRowView<M> IRowView<M>::previousSecure() const { return IRowView<M>(matrix, std::max(index - 1, (size_t)0)); }


template<typename M>
bool IRowView<M>::isFirst() const { return index == 0; }

template<typename M>
bool IRowView<M>::isLast() const { return index == matrix->rows() - 1; }

};
#endif
