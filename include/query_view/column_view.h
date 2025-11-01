#ifndef COLUMN_VIEW_H
#define COLUMN_VIEW_H

#include <variant>
#include <ctime>
#include <vector>
#include <Eigen/Dense>



struct ColumnView {
    std::variant<
        std::vector<std::string>, std::vector<time_t>, std::vector<bool>,
        Eigen::VectorXi, Eigen::VectorXf, Eigen::VectorXd , Eigen::VectorX<long long>
        > data;

    std::string name;

public:
    size_t size() const;

    template<typename T>
    T& operator()(size_t i);

    template<typename VecT>
    VecT* column();
};


#endif
