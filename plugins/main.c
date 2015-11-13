
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "../config.h"
#include "main.h"
#include "../plugin.h"

static plugin_interface_t my_interface;
FILE *my_log;
static void temu_fake_function() { }

static void set_nic_target_port(void* opaque)
{
  printf("APPLE nic_target_port\n");
  printf("%i\n", (int *)opaque);
  printf("APPLE nic_target_port\n");
}

static mon_cmd_t my_term_cmds[] = {
  {     
    .name       = "nic_target_port",
    .args_type  = "port:I",
    .params     = "port",
    .temu       = set_nic_target_port,
    .user_print = temu_fake_function,
    .help       = "set nic_target_port"
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


