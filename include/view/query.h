#ifndef QUERY_VIEW_H
#define QUERY_VIEW_H

#include <iostream>
#include <string>
#include "mysql/connection_pool.h"
#include "datatype/datetime.h"
#include "datastruct/matrix.h"
#include "datastruct/row.h"
#include "view/baseview.h"



namespace sqlxeigen::view {

class Query : public BaseView {
private:

    std::string _querySelect;
    std::string _querySelectWhere;
    std::string _querySelectOrderBy;
    int _querySelectLimit;  // 0 if limit is not ment to be used

public:
    Query(const std::string& Querystr, const std::string& Database);

public: // mysql
    void setWhere(const std::string& Where);
    void setOrderBy(const std::string& Order);
    void setLimit(int Limit);
    void executeSelect();

protected:
    std::string _buildQuery();
};

};
#endif
