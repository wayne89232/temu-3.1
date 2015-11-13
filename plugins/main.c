
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "../config.h"
#include "main.h"
#include "../plugin.h"

static plugin_interface_t my_interface;
FILE *my_log;
const char *nic_target_port = NULL;

int target_port = 3000;

typedef struct mon_cmd_t {
    const char *name;
    const char *args_type;
    const char *params;
    const char *help;
    union {
        void (*cmd);
    } mhandler;
} mon_cmd_t;

static void temp_function()
{
  printf("nic_target_port\n");
}

static mon_cmd_t my_term_cmds[] = {
  {     
    .name       = "temp_function",
    .args_type  = "",
    .params     = "",
    .help       = "temp_function",
    .mhandler.cmd = temp_function
  },
  {NULL, NULL},
};

static void test()
{
  printf("test\n");
}

static void do_set_nic_target_port(int port_num){
    target_port = port_num;
    printf("target port: %d\n", target_port);
}


plugin_interface_t * init_plugin()
{
  if (!(my_log = fopen("plugin.log", "w"))) {
    fprintf(stderr, "cannot create plugin.log\n");
    return NULL;
  }

  // my_interface.term_cmds = my_term_cmds;

  my_interface.test = test;
  my_interface.set_nic_target_port = do_set_nic_target_port;
  return &my_interface;
}


