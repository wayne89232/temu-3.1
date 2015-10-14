#include "config.h"
#include <dlfcn.h>
#include <assert.h>
#include "plugin.h"


plugin_interface_t *temu_plugin = NULL;
static FILE *load_plugin_log = NULL;
static void *plugin_handle = NULL;
static char cur_plugin_path[100]="";

void do_load_plugin(const char *plugin_path)
{
    plugin_interface_t *(*init_plugin) (void);
    char *error;

    //allow multiple plugins?
    // if(cur_plugin_path[0]){
    //     term_printf("%s has already been loaded! \n", plugin_path);
    //   return;
    // }

    plugin_handle = dlopen(plugin_path, RTLD_NOW);
    if(NULL == plugin_handle) {
        term_printf("%s\n", dlerror());
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
        term_printf("fail to initialize the plugin!\n");
        dlclose(plugin_handle);
        plugin_handle = NULL;
        return;
    }

    load_plugin_log = fopen("plugin_log.log", "w");
    assert(load_plugin_log != NULL);
 
    strncpy(plugin_path, plugin_path, PATH_MAX);
    term_printf("%s is loaded successfully!\n", plugin_path);
}

void do_unload_plugin()
{
    if(cur_plugin_path[0]) {
        // plugin->plugin_cleanup();
        fclose(load_plugin_log);
        load_plugin_log = NULL;
        dlclose(plugin_handle);
        plugin_handle = NULL;
        plugin = NULL;

        term_printf("%s is unloaded!\n", plugin_path);
        plugin_path[0] = 0;
    }
}