#ifndef TWDATA_H_
#define TWDATA_H_

#define MAX_PLAT 3

typedef struct {
    float *items;
    size_t count;
    size_t capacity;
} CannonPlatsTimeInfo;

typedef struct {
    PlatformRec *items;
    size_t count;
    size_t capacity;
    Vector2 vel;
    float timer;
    CannonPlatsTimeInfo time_info;
} CannonPlats;

static PlatformRec plats[MAX_PLAT] = {
    {
        .pos = (Vector2){ .x = (WINDOW_WIDTH/2) - 128, .y = (WINDOW_HEIGHT/2) + 64 },
        .size = (Vector2){ .x = 256.0f, .y = 4.0f },
    },
    {
        .pos = (Vector2){ .x = (WINDOW_WIDTH/2) + 32, .y = (WINDOW_HEIGHT/2) + 38 },
        .size = (Vector2){ .x = 72.0f, .y = 4.0f },
    },
};

static PlatformRec cannon = {
    .pos = (Vector2){ .x = ((WINDOW_WIDTH/2) - 128) + 256.0f, .y = ((WINDOW_HEIGHT/2) + 64) - 8 },
    .size = (Vector2){ .x = 32.0f, .y = 16.0f },
};

static CannonPlats cannon_plats = {0};

static PlatformRec birth_point = {
    .pos = (Vector2){ 
        .x = ((WINDOW_WIDTH/2) - 128) + 256.0f, 
        .y = ((WINDOW_HEIGHT/2) + 64) - 8 
    },
    .size = (Vector2){ .x = 32.0f, .y = 16.0f },
};

static inline
void DefineTestWorldRects()
{
    for (size_t i = 0; i < MAX_PLAT; ++i) {
        plats[i].def_rect = (Rectangle) {
            .x = plats[i].pos.x,
            .y = plats[i].pos.y,
            .width = plats[i].size.x,
            .height = plats[i].size.y,
        };
    }

    define_rect(birth_point);
}

#define update_cannon_plat_pos(rect) rect->pos = Vector2Add(rect->pos, Vector2Scale(cannon_plats.vel, GetFrameTime()))
#define update_cannon_def_rect(rect)               \
    do {                                    \
        rect->def_rect.x = rect->pos.x;       \
        rect->def_rect.y = rect->pos.y;       \
    } while (0)

static inline
void FireCannonPlats()
{
    cannon_plats.vel.x = 30.0f;
    cannon_plats.vel.y = 0.0f;
    cannon_plats.timer += GetFrameTime();
    da_append(&cannon_plats.time_info, cannon_plats.timer);
    if (cannon_plats.timer >= 2.5f) {
        da_append(&cannon_plats, birth_point);
        cannon_plats.timer = 0.0f;
    }
    da_foreach(PlatformRec, plat, &cannon_plats) {
        update_cannon_plat_pos(plat);
        update_cannon_def_rect(plat);
        da_append(&plat->pos_data, plat->pos);
    }
    da_foreach(PlatformRec, plat, &cannon_plats) {
        DrawRectangleV(plat->pos, plat->size, GREEN);
    }
}

static inline
void RewindCannonPlats()
{
    cannon_plats.timer = da_last(&cannon_plats.time_info);
    da_remove_unordered(&cannon_plats.time_info, cannon_plats.time_info.count-1);
    da_foreach(PlatformRec, plat, &cannon_plats) {
        if (plat->pos_data.count < 1) {
            size_t index = plat - cannon_plats.items;
            da_remove_unordered(&cannon_plats, index);
        } else {
            plat->pos = da_last(&plat->pos_data);
            da_remove_unordered(&plat->pos_data, plat->pos_data.count-1);
        }
        DrawRectangleV(plat->pos, plat->size, GREEN);
    }
}

static inline
void DefineEntityPlatformBoundaries()
{
    for (size_t i = 0; i < MAX_PLAT; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            Rectangle temp_rect = {0};
            if (j == 0) {
                temp_rect = (Rectangle) {
                    .x = plats[i].pos.x - 2.0f,
                    .y = plats[i].pos.y - entity.size.y,
                    .width = 2.0f,
                    .height = entity.size.y,
                };
            } else {
                temp_rect = (Rectangle) {
                    .x = plats[i].pos.x + plats[i].size.x,
                    .y = plats[i].pos.y - entity.size.y,
                    .width = 2.0f,
                    .height = entity.size.y,
                };
            }
            plats[i].entity_bounds[j] = temp_rect;
        }
    }
}

#endif // TWDATA_H_
