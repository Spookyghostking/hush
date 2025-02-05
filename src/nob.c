#define NOB_IMPLEMENTATION
#include"nob.h"

int main(void) {
    NOB_GO_REBUILD_YOURSELF();

    nob_Cmd cmd = {0};
    
    nob_cmd_append(&cmd, "gcc");
    nob_cmd_append(&cmd, "-o", "hush", "hush.c");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb", "-pedantic");

    nob_cmd_show(cmd);
    if (!nob_cmd_run_sync(cmd, NULL, (Fd*)stderr)) return 1;

    free(cmd.items);

    nob_cmd_clear(&cmd);

    nob_cmd_append(&cmd, "gcc");
    nob_cmd_append(&cmd, "-o", "tests", "tests.c");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-ggdb", "-pedantic");

    nob_cmd_show(cmd);
    if (!nob_cmd_run_sync(cmd, NULL, (Fd*)stderr)) return 1;

    return 0;
}
