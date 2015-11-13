
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
        void (*cmd)(Monitor *mon, const QDict *qdict);
    } mhandler;
} mon_cmd_t;

typedef struct QDict {
    // QObject_HEAD;
    // size_t size;
    // QLIST_HEAD(,QDictEntry) table[QDICT_BUCKET_MAX];
} QDict;


struct Monitor {
    // CharDriverState *chr;
    // int reset_seen;
    // int flags;
    // int suspend_cnt;
    // bool skip_flush;

    // QemuMutex out_lock;
    // QString *outbuf;
    // guint out_watch;

    // /* Read under either BQL or out_lock, written with BQL+out_lock.  */
    // int mux_out;

    // ReadLineState *rs;
    // MonitorControl *mc;
    // CPUState *mon_cpu;
    // BlockCompletionFunc *password_completion_cb;
    // void *password_opaque;
    // mon_cmd_t *cmd_table;
    // QError *error;
    // QLIST_HEAD(,mon_fd_t) fds;
    // QLIST_ENTRY(Monitor) entry;
};


static void set_nic_target_port()
{
  printf("nic_target_port\n");
}

static mon_cmd_t my_term_cmds[] = {
  {     
    .name       = "nic_target_port",
    .args_type  = "",
    .params     = "",
    .help       = "set nic_target_port",
    .mhandler.cmd = set_nic_target_port
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


