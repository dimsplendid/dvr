#include <stdbool.h>

#define NOB_IMPLEMENTATION
#define NOB_STRIP_PREFIX
#include "nob.h"

bool run = false;
bool always_build = false;
const char *source_path = "main.c";
const char *bin_path = "dvr.exe";

const char *plug_path = "plug.c";
const char *plug_target = "plug.dll";

// prelude
void cc(Cmd *cmd) {
    cmd_append(cmd, "gcc");
    cmd_append(cmd, "-Wall", "-Wextra", "-ggdb");
    cmd_append(cmd, "-I", "./raylib-5.5_win64_mingw-w64/include");
}

// epilogue
void libs(Cmd *cmd) {
    cmd_append(cmd, "-lm");
#ifdef _WIN32
    cmd_append(cmd, "-L", "./raylib-5.5_win64_mingw-w64/lib");
    cmd_append(cmd, "-l:raylib.dll");
    cmd_append(cmd, "-l", "gdi32"); 
    cmd_append(cmd, "-l", "winmm");
#else
    cmd_append(cmd, "-Wl,-rpath,./raylib-5.5_win64_mingw-w64/lib"); 
    cmd_append(cmd, "-Wl,-rpath,./"); 
    cmd_append(cmd, "-l:raylib.so");
#endif // _WIN32
}

bool build_plug(Cmd *cmd) {
    if (!always_build && !needs_rebuild1(plug_target, plug_path)) {   
        nob_log(INFO, "%s is up to date", plug_target);
        return true;
    }
    if (file_exists(plug_target)) {
        cmd_append(cmd, "mv", plug_target, temp_sprintf("%s.old", plug_target));
        if (!cmd_run(cmd)) return false;
    }
    cc(cmd);
    cmd_append(cmd, "-fPIC", "-shared");
    cmd_append(cmd, "-o", plug_target);
    cmd_append(cmd, plug_path);
    libs(cmd);
    return cmd_run(cmd);
}

bool build_main(Cmd *cmd) {
    if (!always_build && !needs_rebuild1(bin_path, source_path)) {   
        nob_log(INFO, "%s is up to date", bin_path);
        return true;
    }
    cc(cmd);
    cmd_append(cmd, "-o", bin_path);
    cmd_append(cmd, source_path);
    cmd_append(cmd, "-ldl"); // dlfcn.h
    // cmd_append(cmd, "-O3");
    cmd_append(cmd, "-L./");
    cmd_append(cmd, temp_sprintf("-l:%s", plug_target));
    libs(cmd);
    return cmd_run(cmd);
}

bool build_run(Cmd *cmd) {
    printf("------------------------------\n");
    cmd_append(cmd, temp_sprintf("./%s", bin_path));
    return cmd_run(cmd);
}

int main(int argc, char **argv) {
    GO_REBUILD_URSELF(argc, argv);
    const char *program_name = shift(argv, argc);
    // registed command
    while(argc > 0) {
        const char *flag = shift(argv, argc);
        if (strcmp(flag, "--run"         ) == 0) { run          = true; continue; }
        if (strcmp(flag, "-r"            ) == 0) { run          = true; continue; }
        if (strcmp(flag, "--always-build") == 0) { always_build = true; continue; }
        if (strcmp(flag, "-B"            ) == 0) { always_build = true; continue; }
        fprintf(stderr, "%s:%d: ERROR: unknown flag `%s`\n", __FILE__, __LINE__, flag);
        return 1;
    }
    
    Cmd cmd = {0};
    build_plug(&cmd);
    build_main(&cmd);
    if (run && build_run(&cmd)) return 1;

    cmd_free(cmd);
    return 0;
}
