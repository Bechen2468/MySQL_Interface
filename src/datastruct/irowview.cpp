#include "datastruct/irowview.h"


namespace sqlxeigen {

Matrix::RowView Matrix::rowView(size_t index)       { return RowView(this->shared_from_this(), index); }
SqlMatrix::RowView SqlMatrix::rowView(size_t index) { return RowView(this->shared_from_this(), index); }

};
