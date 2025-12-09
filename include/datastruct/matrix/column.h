#ifndef SQLXEIGEN_COLUMN_H
#define SQLXEIGEN_COLUMN_H

#include <iostream>
#include <string>
#include <variant>
#include <chrono>
#include <vector>
#include <Eigen/Dense>
#include "datastruct/matrix/icolumn.h"


namespace sqlxeigen {
template<typename T>
struct Column : public IColumn {

protected:
    std::vector<T, Eigen::aligned_allocator<T>> _data;
    const Type _type;

public:
    Column(size_t size = 0);
    Column(const std::vector<T>& fromVec);
    
    size_t size() const override;

    Type type() const override;

    void resize(size_t size) override;

    void reserve(size_t resSize) override;

    inline T& get(size_t i);

    std::vector<T, Eigen::aligned_allocator<T>>& asVectorRef();

    Eigen::Map<Eigen::Vector<T, Eigen::DynamicIndex>> asEigenRef();
};



template<typename T>
Column<T>::Column(const std::vector<T>& fromVec):
_type(utils::type::fromRealType<T>()),
_data(fromVec.begin(), fromVec.end())
{}


template<typename T>
inline T& Column<T>::get(size_t i) { 
    return _data[i]; 
}


template<typename T>
std::vector<T, Eigen::aligned_allocator<T>>& Column<T>::asVectorRef() {
    return _data;
}


template<typename T>
Eigen::Map<Eigen::Vector<T, Eigen::DynamicIndex>> Column<T>::asEigenRef() {
    return Eigen::Map<Eigen::Vector<T,Eigen::DynamicIndex>>(asVectorRef(), size());
}


template<typename T>
Column<T>::Column(size_t size):
_type(utils::type::fromRealType<T>()),
_data(size)
{}


template<typename T>
size_t Column<T>::size() const {
    return _data.size();
}


template<typename T>
Type Column<T>::type() const {
    return _type;
}


template<typename T>
void Column<T>::resize(size_t size) {
    _data.resize(size);
}


template<typename T>
void Column<T>::reserve(size_t resSize) {
    _data.reserve(resSize);
}


};
#endif

