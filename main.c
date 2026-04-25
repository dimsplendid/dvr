#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <raylib.h>

#include "plug.h"
#if __APPLE__
const char *libplug_path = "libplug.dylib";
#else
const char *libplug_path = "plug.dll";
#endif
void *libplug = NULL;

bool reload_libplug(void) {
    if (libplug != NULL) dlclose(libplug);
    
    libplug = dlopen(libplug_path, RTLD_NOW);
    
    if (libplug == NULL) {
        fprintf(stderr, "ERROR: %s\n", dlerror());
        return false;
    }
    plug_init = dlsym(libplug, "plug_init");
    if (plug_init == NULL) {
        fprintf(stderr, "ERROR: %s\n", dlerror());
        return false;
    }
    plug_pre_reload = dlsym(libplug, "plug_pre_reload");
    if (plug_pre_reload == NULL) {
        fprintf(stderr, "ERROR: %s\n", dlerror());
        return false;
    }
    plug_post_reload = dlsym(libplug, "plug_post_reload");
    if (plug_post_reload == NULL) {
        fprintf(stderr, "ERROR: %s\n", dlerror());
        return false;
    }
    plug_update = dlsym(libplug, "plug_update");
    if (plug_update == NULL) {
        fprintf(stderr, "ERROR: %s\n", dlerror());
        return false;
    }
    return true;
}

int main(void) {

    if (!reload_libplug()) return 1;

    SetTargetFPS(60);
    plug_init();
    
    InitWindow(800, 600, "DVR");
    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_R)) {
            void *state = plug_pre_reload();
            reload_libplug();
            plug_post_reload(state);
        }
        plug_update();
    }
    CloseWindow();
        
    return 0;
}
