#include "datastruct/column.h"

namespace sqlxeigen::matrix {

Column::Column(mysqlx::Type mysqlType, size_t size, const std::string& name):
_mysqlType(mysqlType),
_name(name)
{
    switch(type()) {
        case Column::Type::STRING:  _data = std::vector<std::string, Eigen::aligned_allocator<std::string>>(size);     break;
        case Column::Type::UINT8:   _data = std::vector<uint8_t, Eigen::aligned_allocator<uint8_t>>(size);      break;
        case Column::Type::INT:     _data = std::vector<int, Eigen::aligned_allocator<int>>(size);  break;
        case Column::Type::UINT64:  _data = std::vector<uint64_t, Eigen::aligned_allocator<uint64_t>>(size);  break;
        case Column::Type::INT64:   _data = std::vector<int64_t, Eigen::aligned_allocator<int64_t>>(size);  break;
        case Column::Type::FLOAT:   _data = std::vector<float, Eigen::aligned_allocator<float>>(size);  break;
        case Column::Type::DOUBLE:  _data = std::vector<double, Eigen::aligned_allocator<double>>(size);  break;
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





};