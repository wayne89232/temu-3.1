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
typedef struct plugin_interface_t{

    // array of terminal commands
    void *term_cmds; 
    void *info_cmds; 
    long long cr3_addr;
    int taint_record_size;

    void (*get_cr3) (void); 

    // void (*taint_disk) (int size, int64_t sect_num, int flag,uint32_t paddr);
    // void (*nic_recv) (uint8_t * buf, int size, int cur_pos, int start, int stop);
    // void (*nic_send) (uint32_t addr, int size, uint8_t * buf);

    void (*test) (void);

} plugin_interface_t;
extern plugin_interface_t *plugin;

void do_load_plugin(const char *plugin_path,const char *port_num);
void do_unload_plugin(void);
