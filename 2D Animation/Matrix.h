#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <vector>
#include <iostream>

using namespace std;

class Matrix
{
  private:
    int rows, columns;
    vector< vector<double> > vals;
    Matrix(int r, int c);

  public:
    Matrix(int r, int c, vector< vector<double> >);
    virtual ~Matrix();

    double GetValue(int, int) const;

    Matrix operator*(const Matrix&);
    friend ostream& operator<<(ostream&, const Matrix&);
};

#endif