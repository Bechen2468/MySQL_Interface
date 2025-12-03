#include "datastruct/column-v2.h"

namespace sqlxeigen::matrix {

ColumnV2::ColumnV2(mysqlx::Type mysqlType, size_t size, const std::string& name):
_mysqlType(mysqlType),
_name(name)
{
    switch(type()) {
        case ColumnV2::Type::STRING:  
            _data = std::vector<std::string>(size);     
            _rawPtr = std::get<0>(_data).data();
            _typeSize = sizeof(std::string);
            break;
        case ColumnV2::Type::UINT8:   
            _data = Eigen::VectorX<uint8_t>(size);      
            _rawPtr = std::get<1>(_data).data();
            _typeSize = sizeof(uint8_t);
            break;
        case ColumnV2::Type::INT:     
            _data = Eigen::VectorXi(size);              
            _rawPtr = std::get<2>(_data).data();
            _typeSize = sizeof(int);
            break;
        case ColumnV2::Type::UINT64:  
            _data = Eigen::VectorX<uint64_t>(size);     
            _rawPtr = std::get<3>(_data).data();
            _typeSize = sizeof(uint64_t);
            break;
        case ColumnV2::Type::INT64:   
            _data = Eigen::VectorX<int64_t>(size);    
            _rawPtr = std::get<4>(_data).data();
            _typeSize = sizeof(int64_t);
            break;
        case ColumnV2::Type::FLOAT:   
            _data = Eigen::VectorXf(size);              
            _rawPtr = std::get<5>(_data).data();
            _typeSize = sizeof(float);
            break;
        case ColumnV2::Type::DOUBLE:  
            _data = Eigen::VectorXd(size);              
            _rawPtr = std::get<6>(_data).data();
            _typeSize = sizeof(double);
            break;
    }
}



size_t ColumnV2::size() const {
    return std::visit([](auto const& vec) -> size_t {
        return vec.size();
    }, _data);
}



ColumnV2::Type ColumnV2::type() const {
    switch(_mysqlType) {
            // Cpp vector
        case mysqlx::Type::ENUM:
        case mysqlx::Type::BYTES:
        case mysqlx::Type::TIME:
        case mysqlx::Type::STRING:      return ColumnV2::Type::STRING;
        case mysqlx::Type::TINYINT:
        case mysqlx::Type::BIT:         return ColumnV2::Type::UINT8; 
        // Eigen
        case mysqlx::Type::INT:         return ColumnV2::Type::INT;   
        case mysqlx::Type::FLOAT:       return ColumnV2::Type::FLOAT; 
        case mysqlx::Type::DECIMAL:
        case mysqlx::Type::DOUBLE:      return ColumnV2::Type::DOUBLE;
        case mysqlx::Type::DATE:
        case mysqlx::Type::DATETIME:
        case mysqlx::Type::BIGINT:      return ColumnV2::Type::INT64; 
        default:
            std::cerr << "ERROR: Query mysqlx-Type not implemented. Type: " << _mysqlType << std::endl;
            return matrix::ColumnV2::Type::STRING;
    }
}


mysqlx::Type ColumnV2::mysqlType() const {
    return _mysqlType;
}


std::string ColumnV2::name() const {
    return _name;
}


void* ColumnV2::rawPtr() {
    return _rawPtr;
}


inline std::string& ColumnV2::get_string(size_t i) {
    return std::get<std::vector<std::string>>(_data)[i];
}

};