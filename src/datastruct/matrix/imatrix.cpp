#include "datastruct/matrix/imatrix.h"

namespace sqlxeigen {

IMatrix::IMatrix(const std::vector<Type>& colTypes, size_t size) {
    for(int i = 0; i < colTypes.size(); ++i) {
        switch(colTypes[i]) {
            case Type::FLOAT:  addColumn<float>(size);       break;
            case Type::DOUBLE: addColumn<double>(size);      break;
            case Type::INT:    addColumn<int>(size);         break;
            case Type::INT64:  addColumn<int64_t>(size);     break;
            case Type::UINT64: addColumn<uint64_t>(size);    break;
            case Type::UINT8:  addColumn<uint8_t>(size);     break;
            case Type::STRING: addColumn<std::string>(size); break;
            default:
                throw std::runtime_error("Unknown type");
        }
    }
}


IMatrix::~IMatrix() {}


size_t IMatrix::rows() const {
    return _columns.empty() ? 0 : _columns[0]->size();
}


size_t IMatrix::cols() const {
    return _columns.size();
}


Type IMatrix::columnType(size_t colIndex) const {
    return _columns[colIndex]->type();
}


void IMatrix::resize(size_t size) {
    for(std::shared_ptr<IColumn> col : _columns) {
        col->resize(size);
    }
}


void IMatrix::reserve(size_t resSize) {
    for(std::shared_ptr<IColumn> col : _columns) {
        col->reserve(resSize);
    }
}



std::shared_ptr<IColumn> IMatrix::columnRaw(size_t index) {
    return _columns[index];
}

void IMatrix::eraseColumn(size_t index) {
    _columns.erase(_columns.begin() + index);
}

void IMatrix::swapColumnPositions(size_t i, size_t j) {
    std::swap(_columns[i], _columns[j]);
}

};