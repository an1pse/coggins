#include "coggins.h"

Enemy::Enemy() 
{
    pos = (Vector2){ .x = (WINDOW_WIDTH/2 + 32), .y = (WINDOW_HEIGHT/2 + 38) - 8.0f };
    size = (Vector2){ .x = 8.0f, .y = 8.0f };
    vel = (Vector2){ .x = 80.0f,  .y = 0.0f };
    grounded = true;
}

Player::Player() 
{
    pos = (Vector2){ .x = WINDOW_WIDTH/2, .y = WINDOW_HEIGHT/2 };
    size = (Vector2){ .x = 8.0f, .y = 8.0f };
    vel = (Vector2){ 0.0f, 0.0f };
    grounded = false;

    camera.offset = (Vector2){ (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f }; 
    camera.target = pos;                                           
    camera.zoom = 4.0f; 
    
    def_rect = (Rectangle) {       
        .x = pos.x,                
        .y = pos.y,                
        .width = size.x,           
        .height = size.y,          
    };

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

void Entity::Update() 
{
    def_rect.x = pos.x;       
    def_rect.y = pos.y;       
} 

void Player::Update() 
{
    def_rect.x = pos.x;       
    def_rect.y = pos.y;       

    eyes[0].x = pos.x + 1.7778;               
    eyes[0].y = pos.y + 2.6667;               
    eyes[1].x = pos.x + 2.6667 + 1.7778;      
    eyes[1].y = pos.y + 2.6667;
} 

void Player::RecenterCam() 
{
    camera.offset = (Vector2){ (float)GetScreenWidth()/2.0f, (float)GetScreenHeight()/2.0f }; 
    camera.target = pos;
}

void Entity::Draw() 
{
    DrawRectangleV(pos, size, ORANGE);
}

void Enemy::Draw() 
{
    DrawRectangleV(pos, size, RED);
}

void Player::Draw() 
{
    DrawRectangleV(pos, size, ORANGE);
    for (size_t i = 0; i < 2; ++i) {                
        DrawRectangleRec(eyes[i], BLACK);    
    }                                               
}

void Entity::Rewind() 
{
    auto last_pos = da_last(&pos_data);
    pos = last_pos;
    Update();
    da_remove_unordered(&pos_data, pos_data.count - 1);
}

void Player::Rewind() 
{
    auto last_pos = da_last(&pos_data);
    pos = last_pos;
    Update();
    camera.target = last_pos;
    grounded = da_last(&gdata);
    da_remove_unordered(&pos_data, pos_data.count - 1);
    da_remove_unordered(&gdata, gdata.count - 1);
    vel.y = da_last(&grav_data);
    da_remove_unordered(&grav_data, grav_data.count - 1);
}

void Player::AppendData()  
{
    da_append(&pos_data, pos);
    da_append(&gdata, grounded);
    da_append(&grav_data, vel.y);
}

void Entity::AppendData()  
{
    da_append(&pos_data, pos);
    da_append(&gdata, grounded);
}
