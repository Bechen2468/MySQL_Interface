#ifndef MY_COLUMN2_H
#define MY_COLUMN2_H

#include <iostream>
#include <string>
#include <variant>
#include <chrono>
#include <vector>
#include <mysqlx/xdevapi.h>
#include <Eigen/Dense>
#include "datatype/datetime.h"


namespace sqlxeigen::matrix {
struct ColumnV2 {
    enum Type {
        STRING = 0,
        UINT8 = 1,
        BOOL = 1,
        INT = 2,
        UINT64 = 3,
        INT64 = 4,
        FLOAT = 5,
        DOUBLE = 6
    };

private:
    void* _rawPtr;
    size_t _typeSize;

    std::variant<
        std::vector<std::string>, 
        Eigen::VectorX<uint8_t>,
        Eigen::VectorXi, Eigen::VectorX<uint64_t>, Eigen::VectorX<int64_t>,
        Eigen::VectorXf, Eigen::VectorXd
        > _data;

    std::string _name;
    mysqlx::Type _mysqlType;

public:
    ColumnV2(mysqlx::Type mysqlType, size_t size, const std::string& name = "");

    
    size_t size() const;

    Type type() const;

    mysqlx::Type mysqlType() const;

    std::string name() const;

    void* rawPtr();

    template<typename T>
    inline T& get(size_t i);
    
    inline std::string& get_string(size_t i);

    template<typename VecT>
    VecT& getVector();
};




template<typename T>
inline T& ColumnV2::get(size_t i) {
    return reinterpret_cast<T*>(_rawPtr)[i];
}



template<typename VecT>
VecT& ColumnV2::getVector() {
    return std::get<VecT>(_data);
}

};

#endif
