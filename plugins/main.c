
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "../config.h"
#include "main.h"
#include "../plugin.h"

static plugin_interface_t my_interface;
FILE *my_log;
const char *nic_target_port = NULL;

typedef struct mon_cmd_t {
    const char *name;
    const char *args_type;
    const char *params;
    const char *help;
    union {
        void (*cmd);
        int (*cmd_new);
    } mhandler;
} mon_cmd_t;

static void set_nic_target_port()
{
  printf("nic_target_port\n");
}
static int int_set_nic_target_port()
{
  printf("int nic_target_port\n");
  return 0;
}

static mon_cmd_t my_term_cmds[] = {
  {     
    .name       = "nic_target_port",
    .args_type  = "",
    .params     = "",
    .help       = "set nic_target_port",
    .mhandler.cmd_new = int_set_nic_target_port,
  },
  {NULL, NULL},
};

static void test()
{
  printf("test\n");
}

plugin_interface_t * init_plugin()
{
  if (!(my_log = fopen("plugin.log", "w"))) {
    fprintf(stderr, "cannot create plugin.log\n");
    return NULL;
  }

  my_interface.term_cmds = my_term_cmds;

  my_interface.test = test;
  return &my_interface;
}


