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

typedef struct mon_cmd_t {
    const char *name;
    const char *args_type;
    void (*user_print)(Monitor *mon, const QObject *data);
    void (*temu)(const char params);
    const char *params;
    const char *help;
    union {
        void (*cmd)(Monitor *mon, const QDict *qdict);
    } mhandler;
} mon_cmd_t;

const char *qdict_get_str(const QDict *qdict, const char *key)
{
    // QObject *obj = qdict_get_obj(qdict, key, QTYPE_QSTRING);
    // return qstring_get_str(qobject_to_qstring(obj));
}