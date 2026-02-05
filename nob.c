#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) 
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    Nob_Cmd cmd = {0};

    nob_cc(&cmd);
    nob_cmd_append(&cmd, "-lraylib");
    nob_cc_inputs(&cmd, "coggins.c");
    nob_cc_output(&cmd, "coggins"); 
    if (!nob_cmd_run(&cmd)) return 1;

    return 0;
}

