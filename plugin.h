
// structure for defining a terminal command
typedef struct term_cmd_t {
	// command name
    const char *name; 
    // command argument list
    const char *args_type;
    // command handler
    void (*handler)(void); 
    // parameters of command handler
    const char *params; 
    // help message
    const char *help; 
} term_cmd_t;

//structure for plugin
typedef struct {

  	// array of terminal commands
  	term_cmd_t *term_cmds; 
  	term_cmd_t *info_cmds; 

  	int taint_record_size; 

  	void (*taint_disk) (int size, int64_t sect_num, int flag,uint32_t paddr);
  	void (*nic_recv) (uint8_t * buf, int size, int cur_pos, int start, int stop);
  	void (*nic_send) (uint32_t addr, int size, uint8_t * buf);

  	void (*test) (void);

} plugin_interface_t;

void do_load_plugin(const char *plugin_path);
void do_unload_plugin(void);