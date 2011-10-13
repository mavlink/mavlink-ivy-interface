#include <glib.h>
#include <stdio.h>
#include <stdlib.h>

#include <Ivy/ivy.h>
#include <Ivy/ivyglibloop.h>

#include <string.h>

static void on_Attitude(IvyClientPtr app, void *user_data, int argc, char *argv[]){
    int i=0;
    //guint ac_id = atoi(argv[0]);
    //float estimator_phi = atof(argv[1]);
    //float estimator_psi = atof(argv[2]);
    //float estimator_theta = atof(argv[3]);
    //printf("Attitude %d %f %f %f \n",ac_id, estimator_phi, estimator_theta, estimator_psi);
    //printf("Msg %d %s \n",ac_id, argv[1]);

    if (strcmp(argv[0],"ground") == 0)
    {
      // printf("ground");
    }
    else
    {
      for (i=0; i< argc; i++)
      {
         printf("%s ",argv[i]);
      }  
      printf("\n");
    }
}


int main ( int argc, char** argv) {

  GMainLoop *ml =  g_main_loop_new(NULL, FALSE);

  IvyInit ("Example1", "Example1 READY", NULL, NULL, NULL, NULL);
  IvyBindMsg(on_Attitude, NULL, "^(\\S*) (.*)");

  IvyStart("127.255.255.255");
  
  g_main_loop_run(ml);

  return 0;
}

