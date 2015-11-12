
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
    } mhandler;
} mon_cmd_t;

static void tests()
{
  printf("testing\n");
}

static mon_cmd_t my_term_cmds[] = {
  {"tests", "","",
  "do something", tests},
  {"test2", "","",
  "do something 2", tests},
  {"nic_target_port", "","",
  "set nic_target_port", tests},
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


