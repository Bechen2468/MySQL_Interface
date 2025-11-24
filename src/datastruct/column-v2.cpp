#include "datastruct/column.h"

namespace sqlxeigen::matrix {

Column::Column(mysqlx::Type mysqlType, size_t size, const std::string& name):
_mysqlType(mysqlType),
_name(name)
{
    switch(type()) {
        case Column::Type::STRING:  _data = std::vector<std::string>(size);     break;
        case Column::Type::UINT8:   _data = Eigen::VectorX<uint8_t>(size);      break;
        case Column::Type::INT:     _data = Eigen::VectorXi(size);              break;
        case Column::Type::UINT64:  _data = Eigen::VectorX<uint64_t>(size);     break;
        case Column::Type::INT64:   _data = Eigen::VectorX<long long>(size);    break;
        case Column::Type::FLOAT:   _data = Eigen::VectorXf(size);              break;
        case Column::Type::DOUBLE:  _data = Eigen::VectorXd(size);              break;
    }
}



size_t Column::size() const {
    return std::visit([](auto const& vec) -> size_t {
        return vec.size();
    }, _data);
}



Column::Type Column::type() const {
    switch(_mysqlType) {
            // Cpp vector
        case mysqlx::Type::ENUM:
        case mysqlx::Type::BYTES:
        case mysqlx::Type::TIME:
        case mysqlx::Type::STRING:      return Column::Type::STRING;
        case mysqlx::Type::TINYINT:
        case mysqlx::Type::BIT:         return Column::Type::UINT8; 
        // Eigen
        case mysqlx::Type::INT:         return Column::Type::INT;   
        case mysqlx::Type::FLOAT:       return Column::Type::FLOAT; 
        case mysqlx::Type::DECIMAL:
        case mysqlx::Type::DOUBLE:      return Column::Type::DOUBLE;
        case mysqlx::Type::DATE:
        case mysqlx::Type::DATETIME:
        case mysqlx::Type::BIGINT:      return Column::Type::INT64; 
        default:
            std::cerr << "ERROR: Query mysqlx-Type not implemented. Type: " << _mysqlType << std::endl;
            return matrix::Column::Type::STRING;
    }
}


mysqlx::Type Column::mysqlType() const {
    return _mysqlType;
}


std::string Column::name() const {
    return _name;
}



// Specific get template types
template<> 
std::string& Column::get<std::string>(size_t i) { return std::get<std::vector<std::string>>(_data)[i]; }
template<> 
uint8_t& Column::get<uint8_t>(size_t i)         { return std::get<Eigen::VectorX<uint8_t>>(_data)[i]; }

template<> 
int& Column::get<int>(size_t i)                 { return std::get<Eigen::VectorXi>(_data)[i]; }
template<> 
uint64_t& Column::get<uint64_t>(size_t i)       { return std::get<Eigen::VectorX<uint64_t>>(_data)[i]; }
template<> 
long long& Column::get<long long>(size_t i)     { return std::get<Eigen::VectorX<long long>>(_data)[i]; }

template<> 
float& Column::get<float>(size_t i)             { return std::get<Eigen::VectorXf>(_data)[i]; }
template<> 
double& Column::get<double>(size_t i)           { return std::get<Eigen::VectorXd>(_data)[i]; }


};