#include "coggins.h"

Player player;
Enemy entity;
GameplayMode game_state = GAMEPLAY;

#include "test_world_data.h"

static inline 
void EntityMove()
{
    update_pos(entity);
    update_def_rect(entity);
 
    if (CheckCollisionRecs(entity.def_rect, plats[1].entity_bounds[0]))
        entity.vel.x = 80;
    else if (CheckCollisionRecs(entity.def_rect, plats[1].entity_bounds[1]))
        entity.vel.x = -80;
}

static inline 
void HandleInputs() 
{
    if (IsKeyPressed(KEY_GRAVE)) {
        if (game_state == GAMEPLAY) game_state = EDIT;
        else if (game_state == EDIT) game_state = GAMEPLAY;
    }

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.vel.x = -MOVE_SPEED;
    else if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.vel.x = MOVE_SPEED;
    else player.vel.x = 0;

    if (player.vel.y == 0.0f && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_J))) {
        player.vel.y = -180;
        player.grounded = false;
    }

    if (!player.grounded) player.vel.y += GRAVITY_FACTOR; 

    update_pos(player);
    update_def_rect(player);

   da_foreach(auto, plat, &cannon_plats) {
        if (CheckCollisionRecs(player.def_rect, plat->def_rect)) {
            player.pos.y = plat->pos.y - player.size.y;
            player.vel.x = 30.0f;
            update_pos(player);
            player.grounded = true;
            player.vel.y = 0;
        } else player.grounded = false;
    }

    for (size_t i = 0; i < MAX_PLAT; i++) {
        if (CheckCollisionRecs(player.def_rect, plats[i].def_rect)) {
            player.pos.y = plats[i].pos.y - player.size.y;
            player.grounded = true;
            player.vel.y = 0;
        } else player.grounded = false;
    }
}

static inline
void HandleRewind()
{
    player.Rewind();
    player.Update();
    
    entity.pos = da_last(&entity.pos_data);
    update_def_rect(entity);
    da_remove_unordered(&entity.pos_data, entity.pos_data.count-1);

    player.Draw();
    DrawRectangleV(entity.pos, entity.size, RED);

    draw_plats();
    RewindCannonPlats();
    DrawRectangleV(cannon.pos, cannon.size, BLUE);
}

static inline
void HandleSTC()
{
    HandleInputs();
    EntityMove();
    player.camera.target = player.pos;
    player.Draw();
    player.Update();
    DrawRectangleV(entity.pos, entity.size, RED);
    draw_plats();
    FireCannonPlats();
    DrawRectangleV(cannon.pos, cannon.size, BLUE);
    player.AppendData(); 
    da_append(&entity.pos_data, entity.pos);
}

static inline
void EditMode()
{
    static bool open = true;
    rlImGuiBegin();
    if (ImGui::Begin("Hello Coggins", &open)) {
        ImGui::TextUnformatted(ICON_FA_JEDI);
    } ImGui::End();
    rlImGuiEnd();
}

int main(void)
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(TARGET_FPS);
    rlImGuiSetup(true);
    define_rect(entity);
    DefineTestWorldRects();
    DefineEntityPlatformBoundaries();
    
    while (!WindowShouldClose()) {
        if (IsWindowResized()) player.RecenterCam();

        BeginDrawing();
        ClearBackground(DARKGRAY);
        if (game_state == EDIT) {                                                     
            EditMode();
        } else if (game_state == GAMEPLAY) {                                          
            BeginMode2D(player.camera);

            if ((IsKeyDown(KEY_H) || IsKeyDown(KEY_Z)) && player.pos_data.count > 0)  
                HandleRewind();                                                       
            else                                                                      
                HandleSTC();                                                          

            EndMode2D();
        }
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
