#include "config.h"
#include <dlfcn.h>
#include <assert.h>
#include "sysemu/sysemu.h"
#include "plugin.h"



plugin_interface_t *plugin = NULL;
static FILE *load_plugin_log = NULL;
static void *plugin_handle = NULL;
static char cur_plugin_path[100]="";

void do_load_plugin(const char *plugin_path,const char *port_num)
{
    printf("Start loading plugin \n");
    plugin_interface_t *(*init_plugin) (void);
    char *error;

    //allow multiple plugins?
    // if(cur_plugin_path[0]){
    //     term_printf("%s has already been loaded! \n", plugin_path);
    //   return;
    // }

    plugin_handle = dlopen(plugin_path, RTLD_NOW);
    if(NULL == plugin_handle) {
        printf("%s\n", dlerror());
        return;
    }

    dlerror();

    init_plugin = dlsym(plugin_handle, "init_plugin");
    if ((error = dlerror()) != NULL) {
        fprintf(stderr, "%s\n", error);
        dlclose(plugin_handle);
        plugin_handle = NULL;
        return;
    }

    plugin = init_plugin();
    if (NULL == plugin) {
        printf("fail to initialize the plugin!\n");
        dlclose(plugin_handle);
        plugin_handle = NULL;
        return;
    }
    printf("%s\n", port_num);
    load_plugin_log = fopen("plugin_log.log", "w");
    assert(load_plugin_log != NULL);
 
    strncpy(cur_plugin_path, plugin_path, 100);
    printf("%s is loaded successfully!\n", plugin_path);
}

void do_unload_plugin(void)
{
    if(cur_plugin_path[0]) {
        // plugin->plugin_cleanup();
        fclose(load_plugin_log);
        load_plugin_log = NULL;
        dlclose(plugin_handle);
        plugin_handle = NULL;
        plugin = NULL;

        printf("plugin unloaded!\n");
        cur_plugin_path[0] = 0;
    }
}
