#define NOB_IMPLEMENTATION
#include "nob.h"

#define BUILD_FOLDER "build/"
#define RAYLIB_HEADERS "thirdparty/raylib-5.5_linux_amd64/include/"
#define RAYLIB_STATIC_LIB "thirdparty/raylib-5.5_linux_amd64/lib/libraylib.a"
#define IMGUI_FOLDER "thirdparty/imgui/"
#define RLIMGUI_FOLDER "thirdparty/rlImGui/"
 
int main(int argc, char **argv) 
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;

    Nob_Cmd cmd = {0};
    nob_cmd_append(&cmd, "g++", "-c");
    nob_cmd_append(&cmd, "-I"RAYLIB_HEADERS, "-I"IMGUI_FOLDER, "-I"RLIMGUI_FOLDER,
            "-L"RAYLIB_STATIC_LIB, "-lraylib");
    nob_cc_inputs(&cmd, "src/coggins.cpp");
    nob_cc_output(&cmd, BUILD_FOLDER"coggins.o"); 
    if (!nob_cmd_run(&cmd)) return 1;

    Nob_Cmd data = {0};
    nob_cmd_append(&data, "g++", "-c");
    nob_cmd_append(&data, "-I"RAYLIB_HEADERS, "-I"IMGUI_FOLDER, "-I"RLIMGUI_FOLDER,
            "-L"RAYLIB_STATIC_LIB, "-lraylib");
    nob_cc_inputs(&data, "src/datafuncs.cpp");
    nob_cc_output(&data, BUILD_FOLDER"datafuncs.o"); 
    if (!nob_cmd_run(&data)) return 1;

    Nob_Cmd imgui1 = {0};
    nob_cmd_append(&imgui1, "g++", "-c");
    nob_cmd_append(&imgui1, "-I"IMGUI_FOLDER);
    nob_cc_inputs(&imgui1, IMGUI_FOLDER"imgui.cpp");
    nob_cc_output(&imgui1, BUILD_FOLDER"imgui.o");
    if (!nob_cmd_run(&imgui1)) return 1;

    Nob_Cmd imgui2 = {0};
    nob_cmd_append(&imgui2, "g++", "-c");
    nob_cmd_append(&imgui2, "-I"IMGUI_FOLDER);
    nob_cc_inputs(&imgui2, IMGUI_FOLDER"imgui_draw.cpp");
    nob_cc_output(&imgui2, BUILD_FOLDER"imgui_draw.o");
    if (!nob_cmd_run(&imgui2)) return 1;

    Nob_Cmd imgui3 = {0};
    nob_cmd_append(&imgui3, "g++", "-c");
    nob_cmd_append(&imgui3, "-I"IMGUI_FOLDER);
    nob_cc_inputs(&imgui3, IMGUI_FOLDER"imgui_tables.cpp");
    nob_cc_output(&imgui3, BUILD_FOLDER"imgui_tables.o");
    if (!nob_cmd_run(&imgui3)) return 1;

    Nob_Cmd imgui4 = {0};
    nob_cmd_append(&imgui4, "g++", "-c");
    nob_cmd_append(&imgui4, "-I"IMGUI_FOLDER);
    nob_cc_inputs(&imgui4, IMGUI_FOLDER"imgui_widgets.cpp");
    nob_cc_output(&imgui4, BUILD_FOLDER"imgui_widgets.o");
    if (!nob_cmd_run(&imgui4)) return 1;

    Nob_Cmd rlImGui = {0};
    nob_cmd_append(&rlImGui, "g++", "-c");
    nob_cmd_append(&rlImGui, "-I"IMGUI_FOLDER, "-I"RLIMGUI_FOLDER, 
            "-I"RAYLIB_HEADERS, "-L"RAYLIB_STATIC_LIB, "-lraylib");
    nob_cc_inputs(&rlImGui, RLIMGUI_FOLDER"rlImGui.cpp");
    nob_cc_output(&rlImGui, BUILD_FOLDER"rlImGui.o");
    if (!nob_cmd_run(&rlImGui)) return 1;

    Nob_Cmd combine = {0};
    nob_cmd_append(&combine, "g++");
    nob_cmd_append(&combine, "-I"IMGUI_FOLDER, "-I"RAYLIB_HEADERS, "-I"RLIMGUI_FOLDER,
            "-L"RAYLIB_STATIC_LIB, "-lraylib");
    nob_cc_inputs(&combine, BUILD_FOLDER"coggins.o", BUILD_FOLDER"datafuncs.o", 
            BUILD_FOLDER"imgui.o", BUILD_FOLDER"imgui_draw.o", BUILD_FOLDER"imgui_tables.o",
            BUILD_FOLDER"imgui_widgets.o", BUILD_FOLDER"rlImGui.o");
    nob_cc_output(&combine, BUILD_FOLDER"coggins");
    if (!nob_cmd_run(&combine)) return 1;

    return 0;
}

