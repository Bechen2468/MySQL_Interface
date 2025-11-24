#include "view/baseview.h"



namespace sqlxeigen::view {


BaseView::BaseView(const std::string& databaseName):
_databaseName(databaseName)
{

}


void BaseView::_rebuildMatrix(const mysqlx::Columns& columns, size_t size) {
    std::vector<mysqlx::Type> types;
    std::vector<std::string> names;

    for(auto col = columns.begin(); col != columns.end(); ++col) {
        types.push_back(col->getType());
        names.push_back(col->getColumnName());
    }

    result = matrix::Matrix(types, size, names);
}


void BaseView::_setFromSQLRow(mysqlx::Row& row, size_t index) {
    for(int i = 0; i < result.cols(); ++i) {
        switch(result.column(i).mysqlType()) {
            // Cpp vector
            case mysqlx::Type::ENUM:
            case mysqlx::Type::BYTES:
            case mysqlx::Type::TIME:
            case mysqlx::Type::STRING:      result.get<std::string>(index, i) = row[i].get<std::string>();                      break;
            case mysqlx::Type::TINYINT:
            case mysqlx::Type::BIT:         result.get<uint8_t>(index, i) = row[i].get<bool>();                                 break;
            // Eigen
            case mysqlx::Type::INT:         result.get<int>(index, i) = row[i].get<int>();                                      break;
            case mysqlx::Type::BIGINT:      result.get<long long>(index, i) = row[i].get<long long>();                          break;
            case mysqlx::Type::FLOAT:       result.get<float>(index, i) = row[i].get<float>();                                  break;
            case mysqlx::Type::DECIMAL:
            case mysqlx::Type::DOUBLE:      result.get<double>(index, i) = row[i].get<double>();                                break;
            case mysqlx::Type::DATE:
            case mysqlx::Type::DATETIME: 
            {
                const mysqlx::byte* raw = row[i].getRawBytes().begin();

                datatype::Datetime dt;
                dt.year((raw[1] << 7) | (raw[0] & 0x7f));
                dt.month(raw[2]);
                dt.day(raw[3]);
                dt.hour(raw[4]);
                dt.minute(raw[5]);
                dt.second(raw[6]);
                // microseconds= ((raw[9] << 14) | (raw[8] << 7) | (raw[7] & 0x7f));

                result.get<long long>(index, i) = dt.raw;
                break;
            }
            default:
                std::cerr << "ERROR: Query mysqlx-Type not implemented. Type: " << result.column(i).mysqlType() << std::endl;
                result.get<std::string>(index, i) = row[i].get<std::string>();
                break;
        }
    }
}


std::string BaseView::_escapeString(const std::string& str) {
    std::string clean = "'";

    for(char c : str) {
        switch (c) {
            case '\0': clean += "\\0"; break;
            case '\n': clean += "\\n"; break;
            case '\r': clean += "\\r"; break;
            case '\\': clean += "\\\\"; break;
            case '\'': clean += "\\'"; break;
            case '"':  clean += "\\\""; break;
            case '\x1a': clean += "\\Z"; break;
            default: clean += c;
        }
    }
    return clean + "'";
}


};