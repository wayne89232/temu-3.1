typedef struct mon_cmd_t {
    const char *name;
    const char *args_type;
    void (*user_print)();
    void (*temu)(const char params);
    const char *params;
    const char *help;
    union {
        void (*cmd)();
    } mhandler;
} mon_cmd_t;