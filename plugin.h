// typedef struct mon_cmd_t {
//     const char *name;
//     const char *args_type;
//     const char *params;
//     const char *help;
//     union {
//         void (*cmd);
//     } mhandler;
// } mon_cmd_t;


//structure for plugin
typedef struct plugin_interface_t {

    // array of terminal commands
    void *term_cmds;
    void *info_cmds;
    long long cr3_addr;
    int taint_record_size;
    void (*set_plugin)(const char *property, const char *value );
    void (*toggle_plugin)(const char *property);

    // void (*taint_disk) (int size, int64_t sect_num, int flag,uint32_t paddr);
    void (*nic_recv) (const uint8_t *buf, size_t size);
    void (*nic_send) (const uint8_t *buf, size_t size);

    void (*test) (void);

} plugin_interface_t;
extern plugin_interface_t *plugin;

void set_plugin(const char *property, const char *value );
void toggle_plugin(const char *property);
void do_load_plugin(const char *plugin_path);
void do_unload_plugin(void);
