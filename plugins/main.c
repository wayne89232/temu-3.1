
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// #include "../config.h"
#include "main.h"
#include "../plugin.h"

#include "../include/qapi/qmp/qobject.h"
#include "../include/qapi/qmp/qlist.h"

static plugin_interface_t my_interface;
FILE *my_log;
const char *nic_target_port = NULL;

typedef struct QDict {
    // QObject_HEAD;
    // size_t size;
    // QLIST_HEAD(,QDictEntry) table[QDICT_BUCKET_MAX];
} QDict;

typedef struct QObject {
    // const QType *type;
    // size_t refcnt;
} QObject;

typedef struct Monitor {
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
} Monitor;

static void monitor_user_noop(Monitor *mon, const QObject *data) { }

typedef struct mon_cmd_t {
    const char *name;
    const char *args_type;
    void (*user_print)(Monitor *mon, const QObject *data);
    const char *params;
    const char *help;
    union {
        void (*cmd)(Monitor *mon, const QDict *qdict);
    } mhandler;
} mon_cmd_t;

static void set_nic_target_port(Monitor *mon, const QDict *qdict)
{
  const char *port = qdict_get_str(qdict, "port");
  printf("APPLE nic_target_port\n");
  printf("%s\n",port);
  printf("APPLE nic_target_port\n");
}

static mon_cmd_t my_term_cmds[] = {
  {     
    .name       = "nic_target_port",
    .args_type  = "",
    .params     = "",
    .user_print = monitor_user_noop,
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


