
/*! conversions needed:
 *  ECEF to TGP, supply origin in WGS
 *  TGP to ECEF, supply origin in WGS
 *  ECEF to WGS84 in degrees, minutes, tenth of minutes
 *  WGS84 to ECEF (deg, min, 1/10min)
 *  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 *  be sure to call either setOrigin function before attempting to do TGP related conversions
 */

#ifndef My_Conversions_Class
#define My_Conversions_Class

#include "../math/matrix.h"

/*!
 *	\defgroup earth Earth Coordinate System Transforms
 *	@{
 */



struct ECEFpoint {
	double x, y, z;
};

class Conversions
{
public:
	Conversions();
	virtual ~Conversions(){};

	static void wgs2ecef(double wgs_north, double wgs_east, double h, double* ecef_x, double* ecef_y, double* ecef_z);	// Rounding error of +/- 5m
	static void ecef2wgs(double ecef_x, double ecef_y, double ecef_z, double* wgs_lat, double* wgs_lon, double* h);
	static double ecef2alt(double ecef_x, double ecef_y, double ecef_z);

	static void ecef2utm(double ecef_x, double ecef_y, double ecef_z, double* utm_north, double* utm_east, double *utm_alt, int *utm_zone);
	static void utm2ecef(double utm_north, double utm_east, double utm_alt, int utm_zone, double *ecef_x, double *ecef_y, double *ecef_z);

	static void utm2llh(double utm_north,double utm_east,double utm_alt, int utm_zone, double *lat,double *lon,double *h);

	void wgs2tgp( double wgs_lat, double wgs_lon, double h, double *tgp_x, double *tgp_y, double *tgp_z);
	void tgp2wgs(double tgp_x, double tgp_y, double tgp_z,  double* wgs_lat, double* wgs_lon, double* h);
	void tgp2ecef(double tgp_x, double tgp_y, double tgp_z, double* ecef_x, double* ecef_y, double* ecef_z);	// Perfect Conversion
	void ecef2tgp(double ecef_x, double ecef_y, double ecef_z, double* tgp_x, double* tgp_y, double* tgp_z);
	void ecef2tgp(double ecef_x, double ecef_y, double ecef_z, float* tgp_x, float* tgp_y, float* tgp_z);
	void ecef_int_cm2tgp(int ecef_x, int ecef_y, int ecef_z, float* tgp_x, float* tgp_y, float* tgp_z);
	void tgp2ecef_int_cm(double tgp_x, double tgp_y, double tgp_z, int* ecef_x, int* ecef_y, int* ecef_z);

	void velocity_tgp2ecef(double tgp_vx, double tgp_vy, double tgp_vz, double* ecef_vx, double* ecef_vy, double* ecef_vz);	// Perfect Conversion
	void velocity_ecef2tgp(float ecef_vx, float ecef_vy, float ecef_vz, float* tgp_vx, float* tgp_vy, float* tgp_vz);
	void velocity_ecef_int_cms2tgp(int ecef_vx, int ecef_vy, int ecef_vz, float* tgp_vx, float* tgp_vy, float* tgp_vz);



	void setOriginEcef(double ecef_x, double ecef_y, double ecef_z);
	void setOriginEcef_int_cm(int ecef_x, int ecef_y, int ecef_z);
	void setOriginWgs (double wgs_north, double wgs_east, double h);
	bool hasorigin(void){return originset;};

	static int wgsString2deg(const char * input, double * output);
	static void wgsDeg2string(double input, char * output);
	static void wgsDeg2string2(double input, char * output);

private:
	matrix x0_ecef;
	matrix T_el;
	bool originset;

	static double DX,DY,DZ;
	static double f, re;
};

//! @}

#endif



