
// typedef struct plugin_cmd {
//     const char *name;
//     const char *args_type;
//     const char *params;
//     const char *help;
//     void (*cmd_handler)(void* opaque);
// } plugin_cmd;


// //structure for plugin
// typedef struct plugin_interface_t{

//     // array of terminal commands
//     // void *term_cmds; 
//     // void *info_cmds; 
//     plugin_cmd *term_cmds;
//     long long cr3_addr;
//     int taint_record_size;

//     void (*get_cr3) (void); 

//     // void (*taint_disk) (int size, int64_t sect_num, int flag,uint32_t paddr);
//     // void (*nic_recv) (uint8_t * buf, int size, int cur_pos, int start, int stop);
//     // void (*nic_send) (uint32_t addr, int size, uint8_t * buf);

//     void (*test) (void);

// } plugin_interface_t;
// extern plugin_interface_t *plugin;
// // extern char cur_plugin_path[100];
// // extern void *plugin_handle;

// void do_load_plugin(const char *plugin_path);
// void do_unload_plugin(void);
