#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#include <Ivy/ivy.h>
#include <Ivy/ivyglibloop.h>


#include "udp.h"
#include "interface.h"

float phi, theta, psi;


static void on_Attitude(IvyClientPtr app, void *user_data, int argc, char *argv[]){
  guint ac_id = atoi(argv[0]);
  phi = atof(argv[1]);
  theta = atof(argv[3]);
  psi = atof(argv[2]);
  printf("ATTITUDE ac=%d phi=%f theta=%f psi=%f\n",ac_id, phi, theta, psi);

  udp_send();
}


int main ( int argc, char** argv) {

  udp_init();

  GMainLoop *ml =  g_main_loop_new(NULL, FALSE);

  IvyInit ("mavlink-ivy-interface", "mavlink-ivy-interface READY", NULL, NULL, NULL, NULL);
  IvyBindMsg(on_Attitude, NULL, "^(\\S*) ATTITUDE (\\S*) (\\S*) (\\S*)");

  IvyStart("127.255.255.255");
  
  g_main_loop_run(ml);

  return 0;
}

