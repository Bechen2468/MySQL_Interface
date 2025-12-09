#include "datastruct/sqlmatrix/sqlmatrix.h"


namespace sqlxeigen {
SqlMatrix::LookupTable::LookupTable(const std::vector<std::string>& names)
{
    for(size_t i = 0; i < names.size(); ++i) {
        setName(i, names[i]);
    }
}


const std::string& SqlMatrix::LookupTable::nameAt(size_t index) const {
    return _indexToName[index];
}


size_t SqlMatrix::LookupTable::indexOf(const std::string& name) const {
    return _nameToIndex.find(name)->second;
}



const std::string& SqlMatrix::LookupTable::nameAt_Secure(size_t index) const {
    return index < _indexToName.size() ? _indexToName[index] : INVALID_STRING;
}


int SqlMatrix::LookupTable::indexOf_Secure(const std::string& name) const {
    if(!_nameToIndex.count(name)) {
        std::cerr << "[LookupTable] ERROR: Name \"" << name << "\" does not exist." << std::endl;
        return -1;
    }
    return _nameToIndex.find(name)->second;
}


int SqlMatrix::LookupTable::setName(size_t index, const std::string& name) {
    if(index >= _indexToName.size()) resize(index + 1);

    const std::string prevName = _indexToName[index];

    if(prevName == name) return 1;  // Nothing changes on equal name

    if(_nameToIndex.find(name) != _nameToIndex.end()) return -1;    // Cant set name if already taken

    if(prevName != "") _nameToIndex.erase(prevName);
    _indexToName[index] = name;
    if(name != "")_nameToIndex.emplace(name, index);

    return 1;   // success
}


void SqlMatrix::LookupTable::resize(size_t size) {
    const size_t preSize = _indexToName.size();
    // Remove overreaching entrys (if reducing size)
    for(size_t i = size; i < preSize; ++i) {    // using i = preSize to not underflow into size_t -1
        _nameToIndex.erase(nameAt(i));   
    }

    _indexToName.resize(size, "");
}





SqlMatrix::SqlMatrix(const std::vector<mysqlx::Type>& colTypes, const std::vector<std::string>& colNames, size_t size):
IMatrix(utils::type::fromSqlTypes(colTypes), size),
_colNames(colNames),
_colTypes(colTypes)
{}


std::shared_ptr<IColumn> SqlMatrix::columnRaw(const std::string& name) {
    return columnRaw(_colNames.indexOf(name));
}


mysqlx::Type SqlMatrix::columnSqlType(size_t index) const {
    return _colTypes[index];
}


mysqlx::Type SqlMatrix::columnSqlType(const std::string& name) const {
    return columnSqlType(_colNames.indexOf(name));
}


std::string SqlMatrix::columnName(size_t index) const {
    return _colNames.nameAt(index);
}


int SqlMatrix::renameColumn(size_t index, const std::string colName) {
    return _colNames.setName(index, colName);
}
};

