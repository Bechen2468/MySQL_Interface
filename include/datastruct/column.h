#ifndef MY_COLUMN_H
#define MY_COLUMN_H

#include <iostream>
#include <string>
#include <variant>
#include <chrono>
#include <vector>
#include <mysqlx/xdevapi.h>
#include <Eigen/Dense>
#include "datatype/datetime.h"


namespace sqlxeigen::matrix {
struct Column {
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
    std::variant<
        std::vector<std::string, Eigen::aligned_allocator<std::string>>,
        std::vector<uint8_t, Eigen::aligned_allocator<uint8_t>>,
        std::vector<int, Eigen::aligned_allocator<int>>,   //, Eigen::aligned_allocator<int>
        std::vector<uint64_t, Eigen::aligned_allocator<uint64_t>>,
        std::vector<int64_t, Eigen::aligned_allocator<int64_t>>,
        std::vector<float, Eigen::aligned_allocator<float>>,
        std::vector<double, Eigen::aligned_allocator<double>>
        > _data;

    std::string _name;
    mysqlx::Type _mysqlType;

public:
    Column(mysqlx::Type mysqlType, size_t size, const std::string& name = "");

    
    size_t size() const;

    Type type() const;

    mysqlx::Type mysqlType() const;

    std::string name() const;

    template<typename T>
    inline T& get(size_t i);

    template<typename VecT>
    VecT& raw();
};





template<typename VecT>
VecT& Column::raw() {
    return std::get<VecT>(_data);
}



//template<> 
//inline int& Column::get<int>(size_t i) { return std::get<2>(_data)[i]; }

template<typename T> 
inline T& Column::get(size_t i) { return std::get<std::vector<T, Eigen::aligned_allocator<T>>>(_data)[i]; }

// Specific get template types
//template<> 
//inline std::string& Column::get<std::string>(size_t i) { return std::get<0>(_data)[i]; }
//template<> 
//inline uint8_t& Column::get<uint8_t>(size_t i)         { return std::get<1>(_data)[i]; }
//
//template<> 
//inline int& Column::get<int>(size_t i)                 { return std::get<2>(_data)[i]; }
//template<> 
//inline uint64_t& Column::get<uint64_t>(size_t i)       { return std::get<3>(_data)[i]; }
//template<> 
//inline int64_t& Column::get<int64_t>(size_t i)         { return std::get<4>(_data)[i]; }
//
//template<> 
//inline float& Column::get<float>(size_t i)             { return std::get<5>(_data)[i]; }
//template<> 
//inline double& Column::get<double>(size_t i)           { return std::get<6>(_data)[i]; }

//template<> 
//inline std::string& Column::get<std::string>(size_t i) { return std::get<std::vector<std::string, Eigen::aligned_allocator<std::string>>>(_data)[i]; }
//template<> 
//inline uint8_t& Column::get<uint8_t>(size_t i)         { return std::get<std::vector<uint8_t, Eigen::aligned_allocator<uint8_t>>>(_data)[i]; }
//
//template<> 
//inline int& Column::get<int>(size_t i)                 { return std::get<std::vector<int, Eigen::aligned_allocator<int>>>(_data)[i]; }
//template<> 
//inline uint64_t& Column::get<uint64_t>(size_t i)       { return std::get<std::vector<uint64_t, Eigen::aligned_allocator<uint64_t>>>(_data)[i]; }
//template<> 
//inline int64_t& Column::get<int64_t>(size_t i)         { return std::get<std::vector<int64_t, Eigen::aligned_allocator<int64_t>>>(_data)[i]; }
//
//template<> 
//inline float& Column::get<float>(size_t i)             { return std::get<std::vector<float, Eigen::aligned_allocator<float>>>(_data)[i]; }
//template<> 
//inline double& Column::get<double>(size_t i)           { return std::get<std::vector<double, Eigen::aligned_allocator<double>>>(_data)[i]; }

};

#endif

