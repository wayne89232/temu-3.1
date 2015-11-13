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