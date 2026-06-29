#include <stdbool.h>
#include <stdint.h>

#include <kos.h>

#define SCREEN_WIDTH 640.0f
#define SCREEN_HEIGHT 480.0f
#define PLAYER_SIZE 40.0f
#define PLAYER_SPEED 4.0f
#define ANALOG_DEADZONE 16

KOS_INIT_FLAGS(INIT_DEFAULT);

typedef struct game_state {
    float player_x;
    float player_y;
    bool running;
} game_state_t;

static pvr_poly_hdr_t polygon_header;

static float clampf(float value, float minimum, float maximum) {
    if(value < minimum)
        return minimum;
    if(value > maximum)
        return maximum;
    return value;
}

static void draw_rectangle(float x, float y, float width, float height,
                           float z, uint32_t color) {
    pvr_vertex_t vertex;

    vertex.flags = PVR_CMD_VERTEX;
    vertex.x = x;
    vertex.y = y;
    vertex.z = z;
    vertex.u = 0.0f;
    vertex.v = 0.0f;
    vertex.argb = color;
    vertex.oargb = 0;
    pvr_prim(&vertex, sizeof(vertex));

    vertex.y = y + height;
    pvr_prim(&vertex, sizeof(vertex));

    vertex.x = x + width;
    vertex.y = y;
    pvr_prim(&vertex, sizeof(vertex));

    vertex.flags = PVR_CMD_VERTEX_EOL;
    vertex.y = y + height;
    pvr_prim(&vertex, sizeof(vertex));
}

static void update(game_state_t *game) {
    maple_device_t *controller = maple_enum_type(0, MAPLE_FUNC_CONTROLLER);

    if(controller) {
        cont_state_t *state = (cont_state_t *)maple_dev_status(controller);

        if(state) {
            float move_x = 0.0f;
            float move_y = 0.0f;

            if(state->buttons & CONT_DPAD_LEFT)
                move_x -= 1.0f;
            if(state->buttons & CONT_DPAD_RIGHT)
                move_x += 1.0f;
            if(state->buttons & CONT_DPAD_UP)
                move_y -= 1.0f;
            if(state->buttons & CONT_DPAD_DOWN)
                move_y += 1.0f;

            if(state->joyx < -ANALOG_DEADZONE || state->joyx > ANALOG_DEADZONE)
                move_x = (float)state->joyx / 128.0f;
            if(state->joyy < -ANALOG_DEADZONE || state->joyy > ANALOG_DEADZONE)
                move_y = (float)state->joyy / 128.0f;

            game->player_x += move_x * PLAYER_SPEED;
            game->player_y += move_y * PLAYER_SPEED;

            if(state->buttons & CONT_START)
                game->running = false;
        }
    }

    game->player_x = clampf(game->player_x, 8.0f,
                            SCREEN_WIDTH - PLAYER_SIZE - 8.0f);
    game->player_y = clampf(game->player_y, 8.0f,
                            SCREEN_HEIGHT - PLAYER_SIZE - 8.0f);
}

static void render(const game_state_t *game) {
    pvr_wait_ready();
    pvr_scene_begin();
    pvr_list_begin(PVR_LIST_OP_POLY);
    pvr_prim(&polygon_header, sizeof(polygon_header));

    draw_rectangle(0.0f, 0.0f, SCREEN_WIDTH, 8.0f, 1.0f, 0xff5bd6ff);
    draw_rectangle(0.0f, SCREEN_HEIGHT - 8.0f, SCREEN_WIDTH, 8.0f,
                   1.0f, 0xff5bd6ff);
    draw_rectangle(0.0f, 8.0f, 8.0f, SCREEN_HEIGHT - 16.0f,
                   1.0f, 0xff5bd6ff);
    draw_rectangle(SCREEN_WIDTH - 8.0f, 8.0f, 8.0f,
                   SCREEN_HEIGHT - 16.0f, 1.0f, 0xff5bd6ff);
    draw_rectangle(game->player_x, game->player_y, PLAYER_SIZE, PLAYER_SIZE,
                   2.0f, 0xffffb347);

    pvr_list_finish();
    pvr_scene_finish();
}

static void initialize_graphics(void) {
    pvr_poly_cxt_t context;

    pvr_init_defaults();
    pvr_set_bg_color(0.03f, 0.05f, 0.12f);
    pvr_poly_cxt_col(&context, PVR_LIST_OP_POLY);
    context.gen.shading = PVR_SHADE_FLAT;
    pvr_poly_compile(&polygon_header, &context);
}

int main(void) {
    game_state_t game = {
        .player_x = (SCREEN_WIDTH - PLAYER_SIZE) * 0.5f,
        .player_y = (SCREEN_HEIGHT - PLAYER_SIZE) * 0.5f,
        .running = true,
    };

    initialize_graphics();
    dbglog(DBG_INFO, "DCTest started. Move with D-pad/analog; START exits.\n");

    while(game.running) {
        update(&game);
        render(&game);
    }

    pvr_shutdown();
    return 0;
}
