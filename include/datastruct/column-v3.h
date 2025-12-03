#ifndef MY_COLUMN3_H
#define MY_COLUMN3_H

#include <iostream>
#include <string>
#include <variant>
#include <chrono>
#include <vector>
#include <mysqlx/xdevapi.h>
#include <Eigen/Dense>
#include "datatype/datetime.h"


namespace sqlxeigen::matrix {
struct ColumnV3 {
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

    uint8_t* _uint8Ptr;
    int* _intPtr;
    uint64_t* _ulongPtr;
    int64_t* _longPtr;
    float* _floatPtr;
    double* _doublePtr;


    std::variant<
        std::vector<std::string>, 
        Eigen::VectorX<uint8_t>,
        Eigen::VectorXi, Eigen::VectorX<uint64_t>, Eigen::VectorX<int64_t>,
        Eigen::VectorXf, Eigen::VectorXd
        > _data;

    std::string _name;
    mysqlx::Type _mysqlType;

public:
    ColumnV3(mysqlx::Type mysqlType, size_t size, const std::string& name = "");

    
    size_t size() const;

    Type type() const;

    mysqlx::Type mysqlType() const;

    std::string name() const;


    inline std::string& get_string(size_t i)   { return std::get<std::vector<std::string>>(_data)[i]; };
    inline uint8_t&     get_uint8(size_t i)    { return _uint8Ptr[i]; };
    inline int&         get_int(size_t i)      { return _intPtr[i]; };
    // gcc/clang:  inline __attribute__((always_inline))
    inline uint64_t&    get_uint64_t(size_t i) { return _ulongPtr[i]; };
    inline int64_t&     get_int64_t(size_t i)  { return _longPtr[i]; };
    inline float&       get_float(size_t i)    { return _floatPtr[i]; };
    inline double&      get_double(size_t i)   { return _doublePtr[i]; };

    template<typename VecT>
    VecT& getVector();
};



template<typename VecT>
VecT& ColumnV3::getVector() {
    return std::get<VecT>(_data);
}

};

#endif
