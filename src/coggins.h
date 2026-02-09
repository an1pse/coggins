#ifndef COGGINS_H_
#define COGGINS_H_

#define RAYMATH_STATIC_INLINE
#include <raylib.h>
#include <raymath.h>

#include <imgui.h>
#include <rlImGui.h>
#include "../nob.h"

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 960
#define WINDOW_TITLE "Coggins"
#define TARGET_FPS 144

#define MOVE_SPEED 150
#define GRAVITY_FACTOR 800 * GetFrameTime()

typedef enum {
    EDIT,
    GAMEPLAY,
} GameplayMode;

typedef struct {
    Vector2 *items;
    size_t count;
    size_t capacity;
} PosData;

typedef struct {
    bool *items;
    size_t count;
    size_t capacity;
} GroundData;

typedef struct {
    float *items;
    size_t count;
    size_t capacity;
} GravData;

struct PlayerRec {
    Vector2 pos;
    Vector2 size;
    Vector2 vel;
    bool grounded;
    PosData pos_data;
    GroundData gdata;
    Camera2D camera;
    Rectangle def_rect;
    GravData grav_data;
    Rectangle eyes[2];

    PlayerRec() {
        pos = (Vector2){ .x = WINDOW_WIDTH/2, .y = WINDOW_HEIGHT/2 };
        size = (Vector2){ .x = 8.0f, .y = 8.0f };
        vel = (Vector2){ 0.0f, 0.0f };
        pos_data = {0};
        gdata = {0};
        grounded = false;
    }

    void SetPlayerEyes() {
        eyes[0] = (Rectangle) {              
            .x = pos.x + 1.7778,             
            .y = pos.y + 2.6667,             
            .width = 1.33335,                       
            .height = 2.6667,                       
        };                                          
        eyes[1] = (Rectangle) {              
            .x = pos.x + 2.6667 + 1.7778,
            .y = pos.y + 2.6667,             
            .width = 1.33335,                       
            .height = 2.6667,                       
        };
    }
};

struct EntityRec {
    Vector2 pos;
    Vector2 size;
    Vector2 vel;
    bool grounded;
    PosData pos_data;
    GroundData gdata;
    Rectangle def_rect;

    EntityRec() {
        pos = (Vector2){ .x = (WINDOW_WIDTH/2 + 32), .y = (WINDOW_HEIGHT/2 + 38) - 8.0f };
        size = (Vector2){ .x = 8.0f, .y = 8.0f };
        vel = (Vector2){ .x = 80.0f,  .y = 0.0f };
        pos_data = {0};
        gdata = {0};
        grounded = true;
    }
};

typedef struct {
    Vector2 pos;
    Vector2 size;
    Rectangle entity_bounds[2];
    Rectangle def_rect;
    PosData pos_data;
} PlatformRec;

#if 0
PlayerRec player = {
    .pos = (Vector2){ .x = WINDOW_WIDTH/2, .y = WINDOW_HEIGHT/2 },
    .size = (Vector2){ .x = 8.0f, .y = 8.0f },
    .vel = (Vector2){ 0.0f, 0.0f },
    .pos_data = {0},
    .gdata = {0},
    .grounded = false,
};
#endif
PlayerRec player;
EntityRec entity;

#if 0
EntityRec entity = {
    .pos = (Vector2){ .x = (WINDOW_WIDTH/2 + 32), .y = (WINDOW_HEIGHT/2 + 38) - 8.0f },
    .size = (Vector2){ .x = 8.0f, .y = 8.0f },
    .vel = (Vector2){ 80.0f, 0.0f },
    .pos_data = {0},
    .gdata = {0},
    .grounded = true,
};
#endif

GameplayMode game_state = GAMEPLAY;

#define draw_player_eyes(player)                    \
    for (size_t i = 0; i < 2; ++i) {                \
        DrawRectangleRec(player.eyes[i], BLACK);    \
    }                                               \

#define update_player_eyes_pos(player)                          \
    do {                                                        \
        player.eyes[0].x = player.pos.x + 1.7778;               \
        player.eyes[0].y = player.pos.y + 2.6667;               \
        player.eyes[1].x = player.pos.x + 2.6667 + 1.7778;      \
        player.eyes[1].y = player.pos.y + 2.6667;               \
    } while (0)


#define init_camera(player)                                                          \
    do {                                                                             \
        player.camera.offset = (Vector2){ GetScreenWidth()/2, GetScreenHeight()/2 }; \
        player.camera.target = player.pos;                                           \
        player.camera.zoom = 4.0f;                                                   \
    } while (0)                                                                      

#define define_rect(rect)                   \
    do {                                    \
        rect.def_rect = (Rectangle) {       \
            .x = rect.pos.x,                \
            .y = rect.pos.y,                \
            .width = rect.size.x,           \
            .height = rect.size.y,          \
        };                                  \
    } while (0) 

#define update_def_rect(rect)               \
    do {                                    \
        rect.def_rect.x = rect.pos.x;       \
        rect.def_rect.y = rect.pos.y;       \
    } while (0)

#define update_pos(rect) rect.pos = Vector2Add(rect.pos, Vector2Scale(rect.vel, GetFrameTime()))
#define draw_plats()                                                    \
    do {                                                                \
        for (size_t i = 0; i < MAX_PLAT; ++i) {                         \
            DrawRectangleV(plats[i].pos, plats[i].size, LIGHTGRAY);     \
        }                                                               \
    } while (0)

#include "test_world_data.h"

#endif // COGGINS_H_
