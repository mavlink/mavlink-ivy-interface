#ifndef WIN32
#include <unistd.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "../interface/udp.h"
#include "../interface/interface.h"
#include "Conversions.h"

int aircraft_id = 2;

float phi[256],theta[256],psi[256];
double lat[256],lon[256],h[256];


Conversions ecef;

/*
  udp_send(ac_id);

  double utm_east = ((double)atof(argv[2])) / 100.0;
  double utm_north = ((double)atof(argv[3])) / 100.0;
  double utm_z = ((double)atof(argv[5])) / 1000.0;
  int utm_zone = atoi(argv[10]);
*/

int main ( int argc, char** argv)
{
  if (argc < 2)
  {
    fprintf(stderr, "USE: test IP \n");
    return -1;
  }

  fprintf(stderr,"test-mavlink-ivy-interface forwarding to '%s' \n",argv[1]);

  // dummy test data
  phi[aircraft_id] = 0.2f;
  theta[aircraft_id] = 0.05f;
  psi[aircraft_id] = 1.4f;

  lat[aircraft_id] = 51.1;
  lon[aircraft_id] = 3.2;
  h[aircraft_id] = 120.0;

  udp_init(argv[1]);

  for (int i=0;;i++)
  {
    psi[aircraft_id] += 0.1;
    if (psi[aircraft_id] > 6.28) psi[aircraft_id] = 0;
    lat[aircraft_id] += 0.0001;
#ifdef WIN32
    Sleep(1000);
#else
    usleep(1000000);
#endif
    printf("send=%d\n",i);
    udp_send(aircraft_id);
  }

  return 0;
}
