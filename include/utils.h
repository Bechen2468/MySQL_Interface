#ifndef SQLXEIGEN_UTILS
#define SQLXEIGEN_UTILS

#include <iostream>
#include <ctime>
#include <mysqlx/xdevapi.h>
#include "datatype/datetime.h"


namespace sqlxeigen {
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

namespace utils {
    namespace time {
        
    };
    
    namespace type {
        template<typename T> struct TypeMap {
            static constexpr Type value = Type::STRING;
        };

        template<> struct TypeMap<uint8_t>     { static constexpr Type value = Type::UINT8; };
        template<> struct TypeMap<bool>        { static constexpr Type value = Type::BOOL; };
        template<> struct TypeMap<int>         { static constexpr Type value = Type::INT; };
        template<> struct TypeMap<uint64_t>    { static constexpr Type value = Type::UINT64; };
        template<> struct TypeMap<int64_t>     { static constexpr Type value = Type::INT64; };
        template<> struct TypeMap<float>       { static constexpr Type value = Type::FLOAT; };
        template<> struct TypeMap<double>      { static constexpr Type value = Type::DOUBLE; };
        template<> struct TypeMap<std::string> { static constexpr Type value = Type::STRING; };

        template<typename T>
        inline constexpr Type fromRealType() {
            return TypeMap<T>::value;
        }


        inline constexpr Type fromSqlType(mysqlx::Type type) {
            switch(type) {
                    // Cpp vector
                case mysqlx::Type::ENUM:
                case mysqlx::Type::BYTES:
                case mysqlx::Type::TIME:
                case mysqlx::Type::STRING:      return Type::STRING;
                case mysqlx::Type::TINYINT:
                case mysqlx::Type::BIT:         return Type::UINT8; 
                // Eigen
                case mysqlx::Type::INT:         return Type::INT;   
                case mysqlx::Type::FLOAT:       return Type::FLOAT; 
                case mysqlx::Type::DECIMAL:
                case mysqlx::Type::DOUBLE:      return Type::DOUBLE;
                case mysqlx::Type::DATE:
                case mysqlx::Type::DATETIME:
                case mysqlx::Type::BIGINT:      return Type::INT64; 
                default:
                    std::cerr << "ERROR: Query mysqlx-Type not implemented. Type: " << type << std::endl;
                    return Type::STRING;
            }
        }


        inline std::vector<Type> fromSqlTypes(const std::vector<mysqlx::Type>& sqlTypes) {
            std::vector<Type> types;
            for(const mysqlx::Type t : sqlTypes) {
                types.push_back(fromSqlType(t));
            }
            return types;
        }


        using _realTypes = std::tuple<std::string, uint8_t, int, uint64_t, int64_t, float, double>;

        template<Type T>
        using toRealType = std::tuple_element_t<T, _realTypes>;


        template<mysqlx::Type T>
        using sqlToRealType = toRealType<utils::type::fromSqlType(T)>;
    }
};

};
#endif
