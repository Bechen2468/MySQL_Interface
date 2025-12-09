#ifndef SQLXEIGEN_QUERY_VIEW_H
#define SQLXEIGEN_QUERY_VIEW_H

#include <iostream>
#include <string>
#include "view/baseview.h"



namespace sqlxeigen {

class QueryView : public BaseView {
private:

    std::string _querySelect;
    std::string _querySelectWhere;
    std::string _querySelectOrderBy;
    int _querySelectLimit;  // 0 if limit is not ment to be used

public:
    QueryView(const std::string& Querystr, const std::string& Database);

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
