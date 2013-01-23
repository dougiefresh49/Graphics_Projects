#include "Matrix.h"

Matrix::Matrix(int r, int c)
  : rows(r), columns(c)
{
  vals = vector< vector<double> >(r, vector<double>(c, 0.0));
}

Matrix::Matrix(int r, int c, vector<vector<double> > v)
  : rows(r), columns(c), vals(v)
{
}

Matrix::~Matrix()
{
}

double Matrix::GetValue(int i, int j) const
{
  return vals[i][j];
}

Matrix Matrix::operator*(const Matrix& m)
{
  Matrix result(this->rows, m.columns);

  double sum;

  for (int i = 0; i < this->rows; i++)
  {
    for (int j = 0; j < m.columns; j++)
	{
      sum = 0;

      for (int k = 0; k < m.rows; k++) 
	  {
        sum += this->vals[i][k] * m.vals[k][j];
      }

      result.vals[i][j] = sum;
    }
  }

  return result;
}

ostream& operator<<(ostream& os, const Matrix& m)
{
  for (int i = 0; i < m.rows; i++) 
  {
    for (int j = 0; j < m.columns; j++)
	{
      os << m.vals[i][j] << " ";
    }

    os << endl;
  }

  return os;
}
