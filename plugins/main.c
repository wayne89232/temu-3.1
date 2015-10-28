
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "../config.h"
#include "main.h"
#include "../plugin.h"

static plugin_interface_t my_interface;
FILE *my_log;

static void tests()
{
  printf("testing\n");
}

static term_cmd_t my_term_cmds[] = {
  {"tests", "", tests,
   "", "do something"},
  {NULL, NULL},
};

static term_cmd_t my_info_cmds[] = {
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

  my_interface.test = test;
  return &my_interface;
}


