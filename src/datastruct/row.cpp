#include "datastruct/matrix.h"
#include "datastruct/row.h"


namespace sqlxeigen::matrix {

Row::Row(Matrix& matrix, size_t index):
matrix(matrix),
index(index)
{

}


void Row::operator=(Row r) {
    this->matrix = r.matrix;
    this->index = r.index;
}


Row Row::next() const { return Row(matrix, std::min(index + 1, matrix.rows() - 1)); }

Row Row::previous() const { return Row(matrix, std::max(index, (size_t)0)); }


bool Row::isFirst() const { return index == 0; }

bool Row::isLast() const { return index == matrix.rows() - 1; }




Row Matrix::row(size_t index) { return Row((*this), index); }



};
