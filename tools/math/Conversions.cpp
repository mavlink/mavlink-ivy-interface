#include "./Conversions.h"
#include "./llh_utm/llh_utm.h"


#include <stdio.h>
#include <string.h>
#include <math.h>
#include <algorithm> // STL algorithms class lib

Conversions::Conversions()
: x0_ecef(3,1)
, T_el(3,3)
, originset(false)
{

}

////////////////////////////////////////////////////////////////////////////
// WGS string conversions
///////////////////////////////////////////////////////////////////////////

// The string is only valid when it starts with a numerical digit
// This function converts strings of the following formats to a double:
// degrees..xxx
// degrees:minutes.xxx
// degrees:minutes:seconds.xxx
// the seperator can be any character except for numerical (0-9) and decimal point (.)

int Conversions::wgsString2deg(const char * input , double * output)
{
	// convert lat/lon character string (value) to double degrees
	char degrees[] = "xxxxxxxxx";	// max 359
	char minutes[] = "xxxxxxxxx";	// max 59
	char seconds[] = "xxxxxxxxx";

	double deg;
	double min;
	double sec;
	
	int decimal = 0; 

	//get the degrees from the string, the string has to start with a numerical digit!
	unsigned int i;
	for(i = 0; i < strlen(input); i++){
		char c = input[i];
		if((c > 47 && c < 58) || (c == 46 && decimal == 0) ) degrees[i] = c; // its a  digit or first decimal point	
		else break;		// its a non-digit or second decimal point thus seperator
		if(c == 46) decimal = 1;
	}

	if(i==0) return 999; // the first entry in the string was not a digit or strlen() equals 0
		
	// if a decimal has been encountered ignore the rest of the string.
	if(decimal == 0) // next are the minutes 
	{
		int n = 0;
		for(i++; i<strlen(input); i++){ 		
			char c = input[i];
			if((c > 47 && c < 58) || (c == 46 && decimal == 0)) minutes[n] = c;	// its a  digit	or '.'
			else break;		// its a non-digit or second decimal point thus seperator
			if(c == 46) decimal = 1; 
			n++;
		}
	    	
		//if(n==0) return 998; // invalid entry: multiple seperator chars or strlen() equals i++
		n = 0;	

		if(decimal == 0) 
		{
			for(i++; i<strlen(input); i++){
				char c = input[i];
				if((c > 47 && c < 58) || (c == 46 && decimal == 0)) seconds[n] = c;	// its a  digit	or '.'
				else break;		// its a non-digit or second decimal point thus seperator
				if(c == 46) decimal = 46;
				n++;
			}

			//if(n==0)return 997; // invalid entry: multiple seperator chars or end of string
		}	
	}	
	
 	deg = (double)atof(degrees);
	min = (double)atof(minutes);
	sec = (double)atof(seconds);

	if(deg< 0 || deg>360 || min<0 || min>60 || sec<0 || sec>60)	return 996; // invalid entry

	double angle  =  deg + (min + sec/60.0)/60.0;  
	if(angle > 360) angle -= 360; // assuming angle < 720

	*output = angle;

	return 0;
}


// string format: "DEGREES:MINUTES.XXXX" 
void Conversions::wgsDeg2string(double angle , char * buffer)
{
	// convert double to lat/lon character string -> value
	int deg = (int)floor(angle);
	double min = ((angle-deg)*60);
	//double sec = (((angle-deg)*60)-min)*60;
    
	sprintf(buffer,"%d:%.4f",deg,min);
}


// string format: "DEGREES:MINUTES:SECONDS.XXX" 
void Conversions::wgsDeg2string2(double angle , char * buffer)
{
	// convert double to lat/lon character string -> value
	int deg = (int)floor(angle);
	int min = (int)floor((angle-deg)*60.0);
	double sec = (((angle-deg)*60)-min)*60.0;
    
	sprintf(buffer,"%d:%d:%.3f",deg,min,sec);
}




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// DATUM CONVERSION
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////


#define SQ(x) ((x)*(x))
#define CUBIC(x) ((x)*(x)*(x))


// confirmed good operation... rounding +/- 5m

void Conversions::wgs2ecef(double wgs_north, double wgs_east, double h, double* ecef_x, double* ecef_y, double* ecef_z)

{
	double lat = wgs_north / 180 * 3.1415926535897932384626433832795;
	double lon = wgs_east / 180 * 3.1415926535897932384626433832795;

	// WGS -> ECEF
	double f = 1/298.257223563;		// Ellipsoid's flatness
	double e = sqrt(f*(2-f));		// Ellipsoid's Eccentricity
	double rp = 6356752.31424518;	// [m] ; semiminor axis: radius polar   // b=6356752:31424518
	double re = 6378137;			// [m] ; semimajor axis: radius equator // a=6378137
	double N = re / sqrt(1 - e*e*sin(lat)*sin(lat) );

	*ecef_x = cos(lat) * (N + h) * cos(lon);
	*ecef_y = cos(lat) * (N + h) * sin(lon);
	*ecef_z = sin(lat) * ((rp*rp)/(re*re)*N + h);

}

void Conversions::ecef2tgp(double ecef_x, double ecef_y, double ecef_z, double* tgp_x, double* tgp_y, double* tgp_z)
{
	matrix x_l(3,1), _x_ecef(3,1), x_temp(3,1);


	if (originset)
	{
		// ECEF -> LOCAL
		_x_ecef(0,0,ecef_x);
		_x_ecef(1,0,ecef_y);
		_x_ecef(2,0,ecef_z);

	//	std::cout << "_x_ecef = " << _x_ecef << std::endl;
	//	std::cout << "x0_ecef = " << this->x0_ecef << std::endl;

		x_temp = (_x_ecef - x0_ecef); //T_el * 
		x_l = T_el * x_temp;

	//	std::cout << "x_temp = " << x_temp << std::endl;
	//	std::cout << "T_el = " << T_el << std::endl;
	//	std::cout << "x_l = " << x_l << std::endl;

		*tgp_x=x_l.GetElement(0,0);
		*tgp_y=x_l.GetElement(1,0);
		*tgp_z=x_l.GetElement(2,0);
	}
	else
	{
		*tgp_x=*tgp_y=*tgp_z=0.0;
	}
}

void Conversions::velocity_ecef2tgp(float ecef_vx, float ecef_vy, float ecef_vz, float* tgp_vx, float* tgp_vy, float* tgp_vz)
{
	matrix vx_l(3,1), _vx_ecef(3,1);

	if (originset)
	{
		// ECEF -> LOCAL
		_vx_ecef(0,0,ecef_vx);
		_vx_ecef(1,0,ecef_vy);
		_vx_ecef(2,0,ecef_vz);

		vx_l = T_el * _vx_ecef;

		*tgp_vx = (float) vx_l.GetElement(0,0);
		*tgp_vy = (float) vx_l.GetElement(1,0);
		*tgp_vz = (float) vx_l.GetElement(2,0);
	}
	else
	{
		*tgp_vx=*tgp_vy=*tgp_vz=0.0f;
	}

}



// Confirmed Good Operation: precision +/- 5m

void Conversions::tgp2ecef(double tgp_x, double tgp_y, double tgp_z, double* ecef_x, double* ecef_y, double* ecef_z)
{
	matrix x_l(3,1), _x_ecef(3,1), temp(3,1), T_le(3,3);


	if (originset)
	{

		x_l(0,0,tgp_x);
		x_l(1,0,tgp_y);
		x_l(2,0,tgp_z);

		T_le = transpose(T_el);

	//	cout << "T_le = " << T_le << endl;

		// TGP 2 ECEF
		temp = (T_le * x_l);
		_x_ecef = temp + x0_ecef;

	//	std::cout << "x_ecef = " << _x_ecef << std::endl;
	//	std::cout << "temp = " << temp << std::endl;

		*ecef_x = _x_ecef.GetElement(0,0);
		*ecef_y = _x_ecef.GetElement(1,0);
		*ecef_z = _x_ecef.GetElement(2,0);
	}
	else
	{
		*ecef_x = *ecef_y = *ecef_z = 0;
	}
}

void Conversions::velocity_tgp2ecef(double tgp_vx, double tgp_vy, double tgp_vz, double* ecef_vx, double* ecef_vy, double* ecef_vz)
{
	matrix vx_l(3,1), _vx_ecef(3,1), T_le(3,3);

	if (originset)
	{
		vx_l(0,0,tgp_vx);
		vx_l(1,0,tgp_vy);
		vx_l(2,0,tgp_vz);

		// TGP 2 ECEF
		T_le = transpose(T_el);
		_vx_ecef = (T_le * vx_l);


		*ecef_vx = _vx_ecef.GetElement(0,0);
		*ecef_vy = _vx_ecef.GetElement(1,0);
		*ecef_vz = _vx_ecef.GetElement(2,0);
	}
	else
	{
		*ecef_vx = *ecef_vy = *ecef_vz = 0;
	}
}

// returns the altitude in WGS84
double Conversions::ecef2alt(double ecef_x, double ecef_y, double ecef_z)
{
	double lat, lon, alt;
	ecef2wgs(ecef_x, ecef_y, ecef_z, &lat, &lon, &alt);
	return alt;

}

void Conversions::ecef2wgs(double ecef_x, double ecef_y, double ecef_z, double* wgs_lat, double* wgs_lon, double* altitude)
{
	
	// ECEF 2 Wold Grid System 1984
	double Xe = ecef_x;
	double Ye = ecef_y;
	double Ze = ecef_z;


	// WGS 84 earth shape model
	double f = 1/298.257223563;		// Ellipsoid's flatness
	double e = sqrt(f*(2-f));		// Ellipsoid's Eccentricity
	double rp = 6357752.3142;		// [m] ; semiminor axis: radius polar   // b=6356752:31424518
	double re = 6378137;			// [m] ; semimajor axis: radius equator // a=6378137

	// Earth position from ECEF to NorthEastDown (Local)
	double lon = atan2(Ye,Xe);
	double p  = sqrt( SQ(Xe)+SQ(Ye) );
	double h[2] = {0, 0};
	double lamdum[2] = {0, 0};
	double N[2] = {re, re};
	double alt=0, E, F, G, c, s, P, Q, r0, V, z0, e_a, lat;
	for (int i=0;i<100;i++)
	{
		lamdum[1] = atan( (Ze + SQ(e) * N[0] * sin( lamdum[0] ) ) / p);
		N[1] = re / sqrt(1 - SQ(e) * SQ( sin(lamdum[0]) )   );
		h[1] = p / cos( lamdum[0] ) - N[0];
		if ( (fabs(h[1])-h[0]) == 0 )
		{
			alt=h[1];
			break;
		}
		h[0]=h[1];
		N[0]=N[1];
		lamdum[0] = lamdum[1];
	}
	E = sqrt(re*re-rp*rp);
	F = 54 * SQ(rp) * SQ(Ze);
	G = SQ(p) + (1 - SQ(e)) * SQ(Ze) - SQ(e) * SQ(E);
	c = ( SQ(SQ(e)) * F * SQ(p) ) / CUBIC(G);
	s = pow(( 1 + c + sqrt( SQ(c) + 2 * c) ), (1/3));
	P = F / (3* SQ(s+(1/s)+1) * SQ(G));
	Q = sqrt(1+2* SQ(SQ(e)) * P);
	r0= -(P * SQ(e) * p ) / (1+Q) + sqrt(0.5 * SQ(re) * (1+(1/Q))-(P*(1-SQ(e))*SQ(Ze))/(Q*(1+Q))-0.5 * P * SQ(p));
	// U = sqrt( SQ(p- SQ(e) * r0) + SQ(Ze) );
	V = sqrt( SQ(p- SQ(e) * r0) + (1-SQ(e)) * SQ(Ze) );
	z0= ( SQ(rp) * Ze) / (re * V);
	e_a = re * e / rp;
	lat = atan( ( Ze + SQ(e_a) * z0) / p );

	*wgs_lat  = lat * 180.0 / 3.1415926535897932384626433832795;
	*wgs_lon = lon * 180.0 / 3.1415926535897932384626433832795;
	*altitude  = alt;
}

void Conversions::setOriginWgs(double wgs_north, double wgs_east, double altitude)
{
	double ecef_x, ecef_y, ecef_z;
	
	this->wgs2ecef(wgs_north, wgs_east, altitude, &ecef_x, &ecef_y, &ecef_z);

	this->setOriginEcef(ecef_x, ecef_y, ecef_z);
	this->originset = true;
}

void Conversions::setOriginEcef_int_cm(int ecef_x,int ecef_y,int ecef_z)
{
	setOriginEcef(
			((double)ecef_x) / 100.0,
			((double)ecef_y) / 100.0,
			((double)ecef_z) / 100.0
		);
}


void Conversions::setOriginEcef(double _x0,double _y0,double _z0)
{
	double Xe = _x0;
	double Ye = _y0;
	double Ze = _z0;

		x0_ecef(0,0,_x0);
		x0_ecef(1,0,_y0);
		x0_ecef(2,0,_z0);


		// WGS 84 earth shape model
		double f = 1/298.257223563;		// Ellipsoid's flatness
		double e = sqrt(f*(2-f));		// Ellipsoid's Eccentricity
		double rp = 6357752.3142;		// [m] ; semiminor axis: radius polar   // b=6356752:31424518
		double re = 6378137;			// [m] ; semimajor axis: radius equator // a=6378137

		// Earth position from ECEF to NorthEastDown (Local)
		double lon = atan2(Ye,Xe);
		double p  = sqrt( SQ(Xe)+SQ(Ye) );
		double h[2] = {0, 0};
		double lamdum[2] = {0, 0};
		double N[2] = {re, re};
		double alt=0, E, F, G, c, s, P, Q, r0, V, z0, e_a, lat;
		for (int i=0;i<100;i++)
		{
			lamdum[1] = atan( (Ze + SQ(e) * N[0] * sin( lamdum[0] ) ) / p);
			N[1] = re / sqrt(1 - SQ(e) * SQ( sin(lamdum[0]) )   );
			h[1] = p / cos( lamdum[0] ) - N[0];
			if ( (fabs(h[1])-h[0]) == 0 )
			{
				alt=h[1];
				break;
			}
			h[0]=h[1];
			N[0]=N[1];
			lamdum[0] = lamdum[1];
		}
		E = sqrt(re*re-rp*rp);
		F = 54 * SQ(rp) * SQ(Ze);
		G = SQ(p) + (1 - SQ(e)) * SQ(Ze) - SQ(e) * SQ(E);
		c = ( SQ(SQ(e)) * F * SQ(p) ) / CUBIC(G);
		s = pow(( 1 + c + sqrt( SQ(c) + 2 * c) ), (1/3));
		P = F / (3* SQ(s+(1/s)+1) * SQ(G));
		Q = sqrt(1+2* SQ(SQ(e)) * P);
		r0= -(P * SQ(e) * p ) / (1+Q) + sqrt(0.5 * SQ(re) * (1+(1/Q))-(P*(1-SQ(e))*SQ(Ze))/(Q*(1+Q))-0.5 * P * SQ(p));
		// U = sqrt( SQ(p- SQ(e) * r0) + SQ(Ze) );
		V = sqrt( SQ(p- SQ(e) * r0) + (1-SQ(e)) * SQ(Ze) );
		z0= ( SQ(rp) * Ze) / (re * V);
		e_a = re * e / rp;
		lat = atan( ( Ze + SQ(e_a) * z0) / p );

		// printf("Conversions::setOrigin(): X0[deg] %f N %f E %f h\n",lat*180/3.141596,lon*180/3.141596,alt);

		// Rotation from ECEF to Local
		T_el(0,0,-cos(lon)*sin(lat));
		T_el(1,0,-sin(lon));
		T_el(2,0,-cos(lon)*cos(lat));
		T_el(0,1,-sin(lon)*sin(lat));
		T_el(1,1, cos(lon));
		T_el(2,1,-sin(lon)*cos(lat));
		T_el(0,2, cos(lat));
		T_el(1,2, 0);
		T_el(2,2,-sin(lat));

	this->originset = true;

//	std::cout << "T_el = " << T_el << std::endl;

}

void Conversions::tgp2ecef_int_cm(double tgp_x, double tgp_y, double tgp_z, int* ecef_x, int* ecef_y, int* ecef_z)
{
	struct {double x,y,z;} p_ECEF;
	tgp2ecef(tgp_x, tgp_y, tgp_z, &p_ECEF.x, &p_ECEF.y, &p_ECEF.z);
	*ecef_x = (int)(100.0*p_ECEF.x);
	*ecef_y = (int)(100.0*p_ECEF.y);
	*ecef_z = (int)(100.0*p_ECEF.z);
}

void Conversions::ecef_int_cm2tgp(int ecef_x, int ecef_y, int ecef_z, float* tgp_x, float* tgp_y, float* tgp_z)
{
	ecef2tgp(
		((double)ecef_x)/100.0, 
		((double)ecef_y)/100.0, 
		((double)ecef_z)/100.0, 
		tgp_x, tgp_y, tgp_z);
}


void Conversions::velocity_ecef_int_cms2tgp(int ecef_vx, int ecef_vy, int ecef_vz, float* tgp_vx, float* tgp_vy, float* tgp_vz)
{
	velocity_ecef2tgp(
		((float)ecef_vx)/100.0f, 
		((float)ecef_vy)/100.0f, 
		((float)ecef_vz)/100.0f, 
		tgp_vx, tgp_vy, tgp_vz);
}


void Conversions::ecef2tgp(double ecef_x, double ecef_y, double ecef_z, float* tgp_x, float* tgp_y, float* tgp_z)
{
	double x;
	double y;
	double z;
	ecef2tgp(ecef_x, ecef_y, ecef_z, &x, &y, &z);
	*tgp_x = (float)x;
	*tgp_y = (float)y;
	*tgp_z = (float)z;
}


void Conversions::tgp2wgs(double tgp_x,double tgp_y,double tgp_z,double* wgs_lat,double* wgs_lon,double* h)
{
	double x;
	double y;
	double z;
	this->tgp2ecef(tgp_x, tgp_y, tgp_z, &x, &y, &z);
	this->ecef2wgs(x,y,z,wgs_lat,wgs_lon,h);
}

void Conversions::wgs2tgp( double wgs_lat, double wgs_lon, double h, double *tgp_x, double *tgp_y, double *tgp_z)
{
	double x;
	double y;
	double z;
	this->wgs2ecef(wgs_lat,wgs_lon,h, &x,&y,&z);
	this->ecef2tgp(x,y,z,tgp_x,tgp_y,tgp_z);
}


/*static*/ void Conversions::ecef2utm(double ecef_x,double ecef_y,double ecef_z,double* utm_north,double* utm_east,double *utm_alt, int *utm_zone)
{
	double lat, lon, h;
	double north, east;
	char zone[4];

	ecef2wgs(ecef_x,ecef_y,ecef_z,&lat,&lon,&h);

	int RefEllipsoid = 23;//WGS-84. See list with file "llh_utm.cpp" for id numbers

	LLtoUTM(RefEllipsoid, lat, lon, north, east, zone);

	*utm_alt = h;
	*utm_east = east;
	*utm_north = north;
	*utm_zone = (zone[0]-'0') * 10 + (zone[1] - '0');
}

/*static*/ void Conversions::utm2ecef(double utm_north,double utm_east,double utm_alt, int utm_zone, double *ecef_x,double *ecef_y,double *ecef_z)
{
	double lat, lon, h;
	char zone[4] = "31U";

	sprintf(zone,"%02dU",utm_zone);

	int RefEllipsoid = 23;//WGS-84. See list with file "llh_utm.cpp" for id numbers

	UTMtoLL(RefEllipsoid, utm_north, utm_east, zone, lat, lon);
	h = utm_alt;

	wgs2ecef(lat, lon,h, ecef_x,ecef_y,ecef_z);
}

/*static*/ void Conversions::utm2llh(double utm_north,double utm_east,double utm_alt, int utm_zone, double *lat,double *lon,double *h)
{
	char zone[4] = "31U";

	int RefEllipsoid = 23;//WGS-84. See list with file "llh_utm.cpp" for id numbers

	UTMtoLL(RefEllipsoid, utm_north, utm_east, zone, *lat, *lon);
	*h = utm_alt;
}


