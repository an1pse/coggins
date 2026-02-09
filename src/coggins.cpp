#include "coggins.h"

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

   da_foreach(PlatformRec, plat, &cannon_plats) {
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
    Vector2 last_pos = da_last(&player.pos_data);
    player.pos = last_pos;
    update_def_rect(player);
    player.camera.target = last_pos;
    player.grounded = da_last(&player.gdata);
    da_remove_unordered(&player.pos_data, player.pos_data.count-1); 
    da_remove_unordered(&player.gdata, player.gdata.count-1); 

    entity.pos = da_last(&entity.pos_data);
    update_def_rect(entity);
    da_remove_unordered(&entity.pos_data, entity.pos_data.count-1);

    player.vel.y = da_last(&player.grav_data);
    da_remove_unordered(&player.grav_data, player.grav_data.count-1);

    DrawRectangleV(player.pos, player.size, ORANGE);
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
    DrawRectangleV(player.pos, player.size, ORANGE);
    draw_player_eyes(player);
    update_player_eyes_pos(player);
    DrawRectangleV(entity.pos, entity.size, RED);
    draw_plats();
    FireCannonPlats();
    DrawRectangleV(cannon.pos, cannon.size, BLUE);
    da_append(&player.pos_data, player.pos);
    da_append(&player.gdata, player.grounded);
    da_append(&player.grav_data, player.vel.y);
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
    init_camera(player);
    define_rect(player);
    player.SetPlayerEyes();
    define_rect(entity);
    DefineTestWorldRects();
    DefineEntityPlatformBoundaries();
    
    while (!WindowShouldClose()) {
        if (IsWindowResized()) init_camera(player);

        if (game_state == EDIT) {                                                     
            BeginDrawing();
            ClearBackground(DARKGRAY);
            EditMode();
            EndDrawing();
        } else if (game_state == GAMEPLAY) {                                          
            BeginDrawing();
            ClearBackground(DARKGRAY);
            BeginMode2D(player.camera);

            if ((IsKeyDown(KEY_H) || IsKeyDown(KEY_Z)) && player.pos_data.count > 0)  
                HandleRewind();                                                       
            else                                                                      
                HandleSTC();                                                          

            EndMode2D();
            EndDrawing();
        }
    }

    CloseWindow();
    return 0;
}
