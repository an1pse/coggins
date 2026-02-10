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
#define update_pos(rect) rect.pos = Vector2Add(rect.pos, Vector2Scale(rect.vel, GetFrameTime()))

enum GameplayMode {
    EDIT,
    GAMEPLAY,
};

template <typename T>
struct HandmadeDA {
    T *items;
    size_t count;
    size_t capacity;
};

struct Entity {
    Vector2 pos;
    Vector2 size;
    Vector2 vel;
    bool grounded;
    HandmadeDA<Vector2> pos_data;
    HandmadeDA<bool> gdata;
    Rectangle def_rect;

    virtual void Update();
    virtual void Draw();
    virtual void Rewind(); 
    virtual void AppendData(); 
};

struct Player : public Entity {
    HandmadeDA<float> grav_data;
    Camera2D camera;
    Rectangle eyes[2];

    Player();
    void Update();
    void Draw();
    void Rewind();
    void RecenterCam();
    void AppendData();
};

struct Enemy : public Entity {
    Enemy(); 
    void Draw();
};

struct PlatformRec{
    Vector2 pos;
    Vector2 size;
    Rectangle entity_bounds[2];
    Rectangle def_rect;
    HandmadeDA<Vector2> pos_data;
};

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

#define draw_plats()                                                    \
    do {                                                                \
        for (size_t i = 0; i < MAX_PLAT; ++i) {                         \
            DrawRectangleV(plats[i].pos, plats[i].size, LIGHTGRAY);     \
        }                                                               \
    } while (0)


#endif // COGGINS_H_
