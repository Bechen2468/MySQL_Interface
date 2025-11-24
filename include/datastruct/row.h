#ifndef MY_ROW_H
#define MY_ROW_H

#include <algorithm>
#include "datastruct/matrix.h"


namespace sqlxeigen::matrix {

struct Row {
    Matrix& matrix;
    size_t index;

    Row(Matrix& matrix, size_t index);


    void operator=(Row r);

    template<typename T>
    T& get(size_t colIndex);

    template<typename T>
    T& get(std::string colName);

    Row next() const;

    Row previous() const;

    bool isFirst() const;

    bool isLast() const;
};


template<typename T>
T& Row::get(std::string colName) {
    return matrix.template get<T>(index, colName);
}


template<typename T>
T& Row::get(size_t colIndex) {
    return matrix.template get<T>(index, colIndex);
}

};
#endif
