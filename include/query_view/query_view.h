#ifndef QUERY_VIEW_H
#define QUERY_VIEW_H

#include <string>
#include "mysql/connection_pool.h"
#include "query_view/column_view.h"



class QueryView {
private:
    std::vector<ColumnView> _columns;
    std::unordered_map<std::string, size_t> _nameToIndex;

    std::string _database;
    std::string _querySelect;
    std::string _querySelectWhere;
    std::string _querySelectOrderBy;
    int _querySelectLimit;  // 0 if limit is not ment to be used

public:
    QueryView(const std::string& Query, const std::string& Database);

    size_t rows() const;
    size_t cols() const;

    
    template<typename T>
    T& operator()(size_t Row, const std::string& ColName);

    template<typename T>
    T& operator()(size_t Row, size_t Col);

    
    template<typename VecT>
    VecT& column(const std::string& Name);

    template<typename VecT>
    VecT& column(size_t Index);

    
    // mysql
public:
    void setWhere(std::string Where);
    void setOrderBy(std::string Order);
    void setLimit(int Limit);
    void update();

protected:
    std::string _buildQuery();
};


#endif
