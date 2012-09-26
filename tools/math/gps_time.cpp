/*
      subroutine sowciv(igwk,sow,iyr,imo,idy,ihr,imn,sec)

*** convert gps week/seconds of week to civil

      implicit double precision(a-h,o-z)
      parameter(mjd6jan80=44244)

*** fmjdg computation preserves digits

      mjd  = igwk*7 + sow/86400.0 + mjd6jan80
      fmjd = dmod(sow,86400.0)/86400.0
      call mjdciv(mjd,fmjd,iyr,imo,idy,ihr,imn,sec)

      return
      end
      subroutine mjdciv(mjd,fmjd,iyr,imo,idy,ihr,imn,sec)

*** convert modified julian date to civil date

*** imo in range 1-12, idy in range 1-31
*** only valid in range mar-1900 thru feb-2100
*** ref: hofmann-wellenhof, 2nd ed., pg 34-35
*** operation confirmed for leap years (incl. year 2000)

      implicit double precision(a-h,o-z)

      rjd=mjd+fmjd+2400000.50
      ia=(rjd+0.50)
      ib=ia+1537
      ic=(ib-122.10)/365.250
      id=365.250*ic
      ie=(ib-id)/30.60010

*** the fractional part of a julian day is fractional mjd + 0.5
*** therefore, fractional part of julian day + 0.5 is fractional mjd

      it1=ie*30.60010
      idy=ib-id-it1+fmjd
      it2=ie/14.0
      imo=ie-1-12*it2
      it3=(7+imo)/10.0
      iyr=ic-4715-it3

      tmp=fmjd*24.0
      ihr=tmp
      tmp=(tmp-ihr)*60.0
      imn=tmp
      sec=(tmp-imn)*60.0

      return
*/


//! convert gps week/seconds of week to civil

void sowciv(short gps_week,unsigned int gps_milisec, int &iyr, int &imo, int &idy, int &ihr, int &imn, int &sec)
{

      double mjd6jan80 = 44244.0;

// fmjdg computation preserves digits

	  double igwk = gps_week;
	  double sow = gps_milisec/1000.0;

      double mjd  = igwk*7.0 + sow/86400.0 + mjd6jan80;
      double fmjd = ((double)(((int)sow) % 86400))/86400.0;

      
// convert modified julian date to civil date

/*
*** imo in range 1-12, idy in range 1-31
*** only valid in range mar-1900 thru feb-2100
*** ref: hofmann-wellenhof, 2nd ed., pg 34-35
*** operation confirmed for leap years (incl. year 2000)
*/


      double rjd=mjd+fmjd+2400000.50;
      double ia=(rjd+0.50);
      double ib=ia+1537.0;
      double ic=(ib-122.10)/365.250;
      double id=365.250*ic;
      double ie=(ib-id)/30.60010;

/*
*** the fractional part of a julian day is fractional mjd + 0.5
*** therefore, fractional part of julian day + 0.5 is fractional mjd
*/
      double it1=ie*30.60010;
      idy=(int)(ib-id-it1+fmjd);
      double it2=ie/14.0;
      imo=(int)(ie-1-12*it2);
      double it3=(7+imo)/10.0;
      iyr=(int)(ic-4715-it3);

      double tmp=fmjd*24.0;
      ihr=(int)tmp;
      tmp=(tmp-ihr)*60.0;
      imn=(int)tmp;
      sec=(int)((tmp-imn)*60.0);

}
