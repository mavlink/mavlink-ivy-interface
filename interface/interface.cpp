#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#include <Ivy/ivy.h>
#include <Ivy/ivyglibloop.h>


#include "udp.h"
#include "interface.h"
#include "Conversions.h"

float phi[256], theta[256], psi[256];
double lat[256],lon[256],h[256];

Conversions ecef;

static void on_Attitude(IvyClientPtr app, void *user_data, int argc, char *argv[])
{
  guint ac_id = atoi(argv[0]);

  phi[ac_id] = atof(argv[1]);
  theta[ac_id] = atof(argv[3]);
  psi[ac_id] = atof(argv[2]);

  //fprintf(stderr, "ATTITUDE ac=%d phi=%f theta=%f psi=%f\n",ac_id, phi, theta, psi);

  udp_send(ac_id);
}

static void on_Gps(IvyClientPtr app, void *user_data, int argc, char *argv[])
{
  guint ac_id = atoi(argv[0]);

/*

   <message name="GPS" id="8">
     <field name="mode"       type="uint8"  unit="byte_mask"/>
     <field name="utm_east"   type="int32"  unit="cm" alt_unit="m"/>
     <field name="utm_north"  type="int32"  unit="cm" alt_unit="m"/>
     <field name="course"     type="int16"  unit="decideg" alt_unit="deg"/>
     <field name="alt"        type="int32"  unit="mm" alt_unit="m"/>
     <field name="speed"      type="uint16" unit="cm/s" alt_unit="m/s"/>
     <field name="climb"      type="int16"  unit="cm/s" alt_unit="m/s"/>
     <field name="week"       type="uint16" unit="weeks"/>
     <field name="itow"       type="uint32" unit="ms"/>
     <field name="utm_zone"   type="uint8"/>
     <field name="gps_nb_err" type="uint8"/>
   </message>

*/

  // int mode = atoi(argv[1]);
  double utm_east = ((double)atof(argv[2])) / 100.0;
  double utm_north = ((double)atof(argv[3])) / 100.0;
  double utm_z = ((double)atof(argv[5])) / 1000.0;
  int utm_zone = atoi(argv[10]);

/*
        driver->miniState_msg->groundspeed = protocol->GPS.pprz_speed;
        driver->miniState_msg->psi		=  (short) (((float) protocol->GPS.pprz_course) * f_deg2rad * 1000.0f  );
        driver->miniState_msg->heading	=  (short) (((float) protocol->GPS.pprz_course) * f_deg2rad * 1000.0f  );

        utm_east = ((double) protocol->GPS.pprz_utm_east) / 100.0;
        utm_north = ((double) protocol->GPS.pprz_utm_north) / 100.0;
        utm_z = ((double) protocol->GPS.pprz_alt) / 1000.0;
        utm_zone = protocol->GPS.pprz_utm_zone;
*/
        ecef.utm2llh(utm_north,utm_east,utm_z,utm_zone,&lat[ac_id],&lon[ac_id],&h[ac_id]);

  //fprintf(stderr, "GPS ac=%d %f %f %f %d\n",ac_id, lat, lon, h, utm_zone);


//  udp_send();
}


int main ( int argc, char** argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "USE: mavlink-ivy-interface IP \n");
    return -1;
  }

  fprintf(stderr,"mavlink-ivy-interface forwarding to '%s' \n",argv[1]);

  udp_init(argv[1]);

  GMainLoop *ml =  g_main_loop_new(NULL, FALSE);

  IvyInit ("mavlink-ivy-interface", "mavlink-ivy-interface READY", NULL, NULL, NULL, NULL);
  IvyBindMsg(on_Attitude, NULL, "^(\\S*) ATTITUDE (\\S*) (\\S*) (\\S*)");
  IvyBindMsg(on_Gps, NULL, "^(\\S*) GPS (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*) (\\S*)");

  IvyStart("127.255.255.255");

  g_main_loop_run(ml);

  return 0;
}
