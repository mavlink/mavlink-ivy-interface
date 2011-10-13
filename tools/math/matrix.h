#ifndef _My_Matrix_Class_
#define _My_Matrix_Class_

#include <iostream>

using namespace std;


const int max_size = 50;

class matrix 
{
//  Oud:matrix(Breedte = i)(Hoogte = j)
//	matrix[Rij=i][Kolom=j]
	int rijen, kolommen;
	double r[max_size][max_size];
public:
// Constructor met argument (aantal kolommen en aantal rijen)
	matrix(int _rijen, int _kolommen) {						
		rijen = _rijen;
		if (rijen>max_size) { rijen = max_size;}
		kolommen = _kolommen;								
		if (kolommen>max_size) { kolommen = max_size;}
		for (int i=0; i<rijen; i++) 
		{
			for (int j=0; j<kolommen; j++) 
			{
				r[i][j] = 0.0;
			}
		}
	}
// Copy Constructor met argument (matrix)
	matrix(const matrix &c) {						
		rijen = c.rijen;
		kolommen = c.kolommen;
		for (int i=0; i<rijen; i++) 
		{
			for (int j=0; j<kolommen; j++) 
			{
				r[i][j] = c.r[i][j];
			}
		}
	}
// Constructor zonder argument
	matrix() {rijen = 1; kolommen=1; r[0][0] =1; }						
// Optelling van matrixen defineren

/////////////////////////////////////////
// If you get an internal compiler error here in VC6, install VC6SP6.exe from microsoft
// Visual Studio 6, Service Pack 6 or later
////////////////////////////////////////
	friend matrix operator+ (const matrix &a, const matrix &b) {
	matrix c;
	if ((a.rijen == b.rijen) && (a.kolommen == b.kolommen)) {
		c.rijen = a.rijen;
		c.kolommen = a.kolommen;
		for (int i=0; i<c.rijen; i++) 
		{
			for (int j=0; j<c.kolommen; j++) 
			{
				c.r[i][j] = a.r[i][j] + b.r[i][j];
			}
		}
	} else {
		c.rijen = 0;
		c.kolommen = 0;
		cout << "Deze 2 matrixen kunnen niet bij elkaar worden opgeteld.\n";
	}
	return c;
	}		
// matrixen gelijkstellen defineren
/*	matrix& operator=(const matrix &a) {
	this->rijen = a.rijen;
	this->kolommen = a.kolommen;
	for (int i=0; i<a.rijen; i++) 
	{
		for (int j=0; j<a.kolommen; j++) 
		{
			this->r[i][j] = a.r[i][j];
		}
	}
	return *this;
	}		
*/	
// Aftrekken van matrixen defineren
	friend matrix operator- (const matrix &a, const matrix &b) {
	matrix c;
	if ((a.rijen == b.rijen) && (a.kolommen == b.kolommen)) {
		c.rijen = a.rijen;
		c.kolommen = a.kolommen;
		for (int i=0; i<c.rijen; i++) 
		{
			for (int j=0; j<c.kolommen; j++) 
			{
				c.r[i][j] = a.r[i][j] - b.r[i][j];
			}
		}
	} else {
		c.rijen = 0;
		c.kolommen = 0;
		cout << "Deze 2 matrixen kunnen niet van elkaar worden afgetrokken.\n";
	}
	return c;
	}		
// Vermenigvuldiging van matrixen defineren
	friend matrix operator* (const matrix &a, const matrix &b)  {
	matrix c;
	if (a.kolommen == b.rijen) {
		c.rijen = a.rijen;
		c.kolommen = b.kolommen;
		for (int i=0; i<c.rijen; i++) {
			for (int j=0; j<c.kolommen; j++) {
				c.r[i][j] = 0;
				for (int k = 0; k<a.kolommen;k++) {
					c.r[i][j] += (a.r[i][k] * b.r[k][j]);
				}
			}
		}
	} else {
		c.rijen = 0;
		c.kolommen = 0;
		cout << "Deze 2 matrixen (" << a.rijen << "x" << a.kolommen << ") (" << b.rijen << "x" << b.kolommen << ") kunnen niet met elkaar worden vermenigvuldigd\n";
	}
	return c;
	}	
// Vermenigvuldiging van matrix met scalar
	friend matrix operator* (const matrix &a, const double &b)  {
	matrix c(a);
	for (int i=0; i<a.rijen; i++) {
		for (int j=0; j<a.kolommen; j++) {
			c.r[i][j] *= b;
		}
	}
	return c;
	}	
// Set element
/*	friend double operator()(int rij,int kol)
	{
		if ((rij > this->br) || (rij<0) || (kol<0) || (kol>this->br))
			cout << "Dit element bestaat niet\n";
		else
			return this->r[rij][kol];
		return 0;
	}
*/
	matrix operator()(int rij,int kolom,double value)
	{
		if ((rij >= this->rijen) || (rij<0) || (kolom<0) || (kolom>=this->kolommen))
			cout << "Element bestaat niet\n";
		else
			this->r[rij][kolom] = value;

		return *this;
	}

	void SetElement(int rij, int kolom, double value)
	{
		if ((rij >= this->rijen) || (rij<0) || (kolom<0) || (kolom>=this->kolommen))
			cout << "Element bestaat niet\n";
		else
			this->r[rij][kolom] = value;
	}
// Get element
	double GetElement(int rij, int kolom)
	{
		if ((rij >= this->rijen) || (rij<0) || (kolom<0) || (kolom>=this->kolommen))
		{
			cout << "Element bestaat niet\n";
			return 0.0;
		}
		else
		{
			return this->r[rij][kolom];
		}
	}
// Canonieke matrixen maken
/*	friend matrix rref(const matrix &d) {	
	matrix c(d);
	if (d.br == (d.ho + 1)) {
		for (int j=1; j<=c.ho; j++) {				// Voor alle rijen
			double deel = c.r[j][j];					// Maak een eentje
			if (deel != 0) {
				for (int i = 1; i<=c.br; i++) {
					c.r[i][j] = c.r[i][j] / deel;
				}
			} else {
				cout << "Dit stelsel is niet oplosbaar\n";
			}
			for (int jj = 1; jj<=c.ho; jj++) {		// Maak allemaal nulletjes
				double maal = c.r[j][jj];
				if (jj != j) {
					for (int ii = 1; ii <= c.br; ii++) {
						c.r[ii][jj] = c.r[ii][jj] - maal * c.r[ii][j];
					}
				}
			}
		}
	} else {
		c.br = 0;
		c.ho = 0;
		cout << "Deze matrix kan niet canoniek worden gemaakt\n";
	}
	return c;}
// Geef gereduceerde matrix
	friend matrix gereduceerde(const matrix &e,int rij, int kol) {
		matrix f;
		if (e.br>=1 && e.ho>=1) {
			f.br = e.br - 1;
			f.ho = e.ho - 1;
			int jj=1;
			for (int j = 1; j<=e.ho; j++) {
				if (j != rij) {
					int ii = 1;
					for (int i = 1; i<=e.br; i++) {
						if (i != kol) {
							f.r[ii][jj] = e.r[i][j];
							ii++;
						}
					}
					jj++;
				}
			}
		} else {
			f.br = 0;
			f.ho = 0;
		}
		return f; 
	}
// Bereken determinant
	friend double det(const matrix &b) {
		double de = 0;
		int max;
		if (b.br < b.ho) {
			max = b.br;
		} else {
			max = b.ho;
		}
		switch (max) {
			case 0: de = 1e99; break;
			case 1:	de = b.r[1][1]; break;
			case 2: de = b.r[1][1] * b.r[2][2] - b.r[1][2] * b.r[2][1]; break;
			default: 
				for (int i = 1; i <= max;i++) {
					de = de + teken(1,i) * b.r[i][1] * det( gereduceerde(b,1,i) );
				} 
		}
		return de;}
*/
// Transpose Matrix
	friend matrix transpose(const matrix &z) {
		matrix g(z);
		if (z.rijen == z.kolommen) {
			for (int i=0; i<z.rijen; i++) {
				for (int j=0; j<z.kolommen; j++) {
					g.r[i][j] = z.r[j][i];
				}
			}
		} else {
			cout << "Niet Transposebaar want niet vierkant\n";
			g.rijen = 0;
			g.kolommen = 0;
		}
		return g;
	}
// Bereken inverse matrix
/*	friend matrix inv(const matrix &e) {
		matrix a(e);
		double de = det(e);
		if ((de == 0) || (e.br != e.ho)) {
			a.br = 0;
			a.ho = 0;
		} else {
			for (int i=1; i<=e.br;i++) {
				for (int j=1; j<=e.ho;j++) {
					a.r[i][j] = teken(i,j) * det( gereduceerde(e,j,i) ) / de;
				}
			}
		}
		return transpose(a);
	}
// Return 1 if a+b is even -1 if odd
	static int teken(int a, int b) {
		int te = a + b;
		if (te % 2 == 1) {
			te = -1;
		} else {
			te = 1;
		}
		return te;
	}
// Invoeren van matrix
	friend istream& operator>> (istream &is, matrix &d) {
		int i,j;
		cout << "\nVoer het aantal rijen en kolommen in: Max (" << max_size << "x" << max_size << ")\n";
		is >> j >> i; d.br = i; d.ho = j;
		if ( j > max_size ) { j = max_size; }
		if ( i > max_size ) { i = max_size; }
		cout << "Voer nu de " << i * j << " elementen van de matrix per rij in:\n";
		for (int jj=1; jj<=d.ho; jj++) {
			for (int ii =1; ii<=d.br; ii++) {
				is >> d.r[ii][jj];
			}
		}
		return is;
	}
*/

// Op scherm tonen van matrix
	friend ostream& operator<< (ostream &os, matrix &d)
	{	if (d.rijen == 0 || d.kolommen == 0) {
			os << "Lege matrix (vb na een omogelijke bewerking...) \n";
		} else {
			os  << "Matrix: (" << d.rijen << " x " << d.kolommen  << ") \n";
			for (int i=0; i<d.rijen; i++) {
				for (int j=0; j<d.kolommen; j++) {
					os << "  " << d.r[i][j];
				}
				os << endl;
			}
		}
		return os;
	}
};


#endif



