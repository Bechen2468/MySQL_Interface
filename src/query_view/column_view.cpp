#include <query_view/column_view.h>



size_t ColumnView::size() const {
    return std::visit([](auto const& vec) -> size_t {
        return vec.size();
    }, data);
}


template<typename T>
T& ColumnView::operator()(size_t i) {
    return std::visit([&](auto& vec) -> T& {  // return ref
        using VecT = std::decay_t<decltype(vec)>;
        if constexpr (std::is_base_of_v<Eigen::EigenBase<VecT>, VecT>)
            return vec(i);
        else
            return vec[i];
    }, data);
}


template<typename VecT>
VecT* ColumnView::column() {
    return std::get_if<VecT>(&data);
}
