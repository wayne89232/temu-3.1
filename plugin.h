
//structure for plugin
typedef struct plugin_interface_t {

    // array of terminal commands
    void *term_cmds;
    void *info_cmds;
    long long cr3_addr;
    int taint_record_size;
    void (*reset_plugin)(const char *property);
    void (*set_plugin)(const char *property, const char *value );
    void (*toggle_plugin)(const char *property);

    // void (*taint_disk) (int size, int64_t sect_num, int flag,uint32_t paddr);
    void (*nic_recv) (const uint8_t *buf, size_t size);
    void (*nic_send) (const uint8_t *buf, size_t size);
    void (*blk_write) (uint64_t sector_num, uint64_t base, uint64_t len);
    void (*blk_read) (uint64_t sector_num, uint64_t base, uint64_t len);

    void (*test) (void);

} plugin_interface_t;
extern plugin_interface_t *plugin;

void do_load_plugin(const char *plugin_path);
void do_unload_plugin(void);
