#include "datastruct/column-v3.h"

namespace sqlxeigen::matrix {

ColumnV3::ColumnV3(mysqlx::Type mysqlType, size_t size, const std::string& name):
_mysqlType(mysqlType),
_name(name)
{
    switch(type()) {
        case ColumnV3::Type::STRING:  
            _data = std::vector<std::string>(size);     
            _rawPtr = std::get<0>(_data).data();
            _typeSize = sizeof(std::string);
            break;
        case ColumnV3::Type::UINT8:   
            _data = Eigen::VectorX<uint8_t>(size);      
            _rawPtr = std::get<1>(_data).data();
            _typeSize = sizeof(uint8_t);
            _uint8Ptr = std::get<1>(_data).data();
            break;
        case ColumnV3::Type::INT:     
            _data = Eigen::VectorXi(size);              
            _rawPtr = std::get<2>(_data).data();
            _typeSize = sizeof(int);
            _intPtr = std::get<2>(_data).data();
            break;
        case ColumnV3::Type::UINT64:  
            _data = Eigen::VectorX<uint64_t>(size);     
            _rawPtr = std::get<3>(_data).data();
            _typeSize = sizeof(uint64_t);
            _ulongPtr = std::get<3>(_data).data();
            break;
        case ColumnV3::Type::INT64:   
            _data = Eigen::VectorX<long long>(size);    
            _rawPtr = std::get<4>(_data).data();
            _typeSize = sizeof(long long);
            _longPtr = std::get<4>(_data).data();
            break;
        case ColumnV3::Type::FLOAT:   
            _data = Eigen::VectorXf(size);              
            _rawPtr = std::get<5>(_data).data();
            _typeSize = sizeof(float);
            _floatPtr = std::get<5>(_data).data();
            break;
        case ColumnV3::Type::DOUBLE:  
            _data = Eigen::VectorXd(size);              
            _rawPtr = std::get<6>(_data).data();
            _typeSize = sizeof(double);
            _doublePtr = std::get<6>(_data).data();
            break;
    }
}



size_t ColumnV3::size() const {
    return std::visit([](auto const& vec) -> size_t {
        return vec.size();
    }, _data);
}



ColumnV3::Type ColumnV3::type() const {
    switch(_mysqlType) {
            // Cpp vector
        case mysqlx::Type::ENUM:
        case mysqlx::Type::BYTES:
        case mysqlx::Type::TIME:
        case mysqlx::Type::STRING:      return ColumnV3::Type::STRING;
        case mysqlx::Type::TINYINT:
        case mysqlx::Type::BIT:         return ColumnV3::Type::UINT8; 
        // Eigen
        case mysqlx::Type::INT:         return ColumnV3::Type::INT;   
        case mysqlx::Type::FLOAT:       return ColumnV3::Type::FLOAT; 
        case mysqlx::Type::DECIMAL:
        case mysqlx::Type::DOUBLE:      return ColumnV3::Type::DOUBLE;
        case mysqlx::Type::DATE:
        case mysqlx::Type::DATETIME:
        case mysqlx::Type::BIGINT:      return ColumnV3::Type::INT64; 
        default:
            std::cerr << "ERROR: Query mysqlx-Type not implemented. Type: " << _mysqlType << std::endl;
            return matrix::ColumnV3::Type::STRING;
    }
}


mysqlx::Type ColumnV3::mysqlType() const {
    return _mysqlType;
}


std::string ColumnV3::name() const {
    return _name;
}


};