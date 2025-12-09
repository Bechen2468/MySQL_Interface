#ifndef SQLXEIGEN_ICOLUMN_H
#define SQLXEIGEN_ICOLUMN_H

#include "utils.h"


namespace sqlxeigen {
struct IColumn {
    virtual ~IColumn() = default;
    virtual size_t size() const = 0;
    virtual Type type() const = 0;
    virtual void resize(size_t size) = 0;
    virtual void reserve(size_t resSize) = 0;
};
};
#endif