#ifndef _My_Matrix_Class_
#define _My_Matrix_Class_

#include <iostream>

using namespace std;


const int max_size = 50;

class matrix
{
//  Oud:matrix(Width = i)(Height = j)
//	matrix[Row=i][Column=j]
	int rows, columns;
	double r[max_size][max_size];
public:
// Constructor with argument (number of columns and number of rows)
	matrix(int _rows, int _columns) {
		rows = _rows;
		if (rows>max_size) { rows = max_size;}
		columns = _columns;
		if (columns>max_size) { columns = max_size;}
		for (int i=0; i<rows; i++)
		{
			for (int j=0; j<columns; j++)
			{
				r[i][j] = 0.0;
			}
		}
	}
// Copy Constructor with argument (matrix)
	matrix(const matrix &c) {
		rows = c.rows;
		columns = c.columns;
		for (int i=0; i<rows; i++)
		{
			for (int j=0; j<columns; j++)
			{
				r[i][j] = c.r[i][j];
			}
		}
	}
// Constructor without argument
	matrix() {rows = 1; columns=1; r[0][0] =1; }
// Define Sum of matrix

/////////////////////////////////////////
// If you get an internal compiler error here in VC6, install VC6SP6.exe from microsoft
// Visual Studio 6, Service Pack 6 or later
////////////////////////////////////////
	friend matrix operator+ (const matrix &a, const matrix &b) {
	matrix c;
	if ((a.rows == b.rows) && (a.columns == b.columns)) {
		c.rows = a.rows;
		c.columns = a.columns;
		for (int i=0; i<c.rows; i++)
		{
			for (int j=0; j<c.columns; j++)
			{
				c.r[i][j] = a.r[i][j] + b.r[i][j];
			}
		}
	} else {
		c.rows = 0;
		c.columns = 0;
		cout << "These 2 matrixes can not be added.\n";
	}
	return c;
	}
// Substraction
	friend matrix operator- (const matrix &a, const matrix &b) {
	matrix c;
	if ((a.rows == b.rows) && (a.columns == b.columns)) {
		c.rows = a.rows;
		c.columns = a.columns;
		for (int i=0; i<c.rows; i++)
		{
			for (int j=0; j<c.columns; j++)
			{
				c.r[i][j] = a.r[i][j] - b.r[i][j];
			}
		}
	} else {
		c.rows = 0;
		c.columns = 0;
		cout << "These 2 matrices can not be subtracted.\n";
	}
	return c;
	}
// Multiplication
	friend matrix operator* (const matrix &a, const matrix &b)  {
	matrix c;
	if (a.columns == b.rows) {
		c.rows = a.rows;
		c.columns = b.columns;
		for (int i=0; i<c.rows; i++) {
			for (int j=0; j<c.columns; j++) {
				c.r[i][j] = 0;
				for (int k = 0; k<a.columns;k++) {
					c.r[i][j] += (a.r[i][k] * b.r[k][j]);
				}
			}
		}
	} else {
		c.rows = 0;
		c.columns = 0;
		cout << "These 2 matrices (" << a.rows << "x" << a.columns << ") (" << b.rows << "x" << b.columns << ") can not be multiplied\n";
	}
	return c;
	}
// Scalar Multiplication
	friend matrix operator* (const matrix &a, const double &b)  {
	matrix c(a);
	for (int i=0; i<a.rows; i++) {
		for (int j=0; j<a.columns; j++) {
			c.r[i][j] *= b;
		}
	}
	return c;
	}
	matrix operator()(int Row,int Column,double value)
	{
		if ((Row >= this->rows) || (Row<0) || (Column<0) || (Column>=this->columns))
			cout << "Element does not exist\n";
		else
			this->r[Row][Column] = value;

		return *this;
	}

	void SetElement(int Row, int Column, double value)
	{
		if ((Row >= this->rows) || (Row<0) || (Column<0) || (Column>=this->columns))
			cout << "Element does not exist\n";
		else
			this->r[Row][Column] = value;
	}
// Get element
	double GetElement(int Row, int Column)
	{
		if ((Row >= this->rows) || (Row<0) || (Column<0) || (Column>=this->columns))
		{
			cout << "Element does not exist\n";
			return 0.0;
		}
		else
		{
			return this->r[Row][Column];
		}
	}
// Transposed Matrix
	friend matrix transpose(const matrix &z) {
		matrix g(z);
		if (z.rows == z.columns) {
			for (int i=0; i<z.rows; i++) {
				for (int j=0; j<z.columns; j++) {
					g.r[i][j] = z.r[j][i];
				}
			}
		} else {
			cout << "Transpose of non-square matrix not possible\n";
			g.rows = 0;
			g.columns = 0;
		}
		return g;
	}
// Print matrix
	friend ostream& operator<< (ostream &os, matrix &d)
	{	if (d.rows == 0 || d.columns == 0) {
			os << "Empty matrix (e.g. after error...) \n";
		} else {
			os  << "Matrix: (" << d.rows << " x " << d.columns  << ") \n";
			for (int i=0; i<d.rows; i++) {
				for (int j=0; j<d.columns; j++) {
					os << "  " << d.r[i][j];
				}
				os << endl;
			}
		}
		return os;
	}
};


#endif



