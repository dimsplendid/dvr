#include <assert.h>
#include <stdlib.h>
#include <raylib.h>
#include <math.h>
#include <raymath.h>

#include "nob.h"

typedef struct {
    Color background;
} Plug;

static Plug *p = NULL;

void plug_init(void){
    p = calloc(1, sizeof(*p));
    assert(p != NULL);
    p->background = GREEN;
    TraceLog(LOG_INFO, "--------------------------");
    TraceLog(LOG_INFO, "Initialized dynamic plugin");
    TraceLog(LOG_INFO, "--------------------------");
}
void *plug_pre_reload(void){
    return p;
}
void plug_post_reload(void *state){
    p = state;
}
void plug_update(void){
    float w = GetScreenWidth();
    float h = GetScreenHeight();
    float dt = GetFrameTime(); UNUSED(dt);
    float t = GetTime();
    size_t rect_num = 200;
    float rw = Clamp(w/rect_num-2 , 4, w);
    float rh = rw;
    
    BeginDrawing();
    ClearBackground(GetColor(0x181818FF));
    for (size_t i = 0; i < rect_num; ++i) {
        DrawRectangle(
            w * (float)i / (float)rect_num,
            (h-rh)/ 2.0f + h/2.0f*(sinf(2*t + 1.0f * i / PI)) * (float)i / (float)rect_num,
            rw, rh,
            GREEN);
            // RAYWHITE);
    }
    // DrawRectangle((w-rw)*(sinf(t) + 1.0f) / 2.0f, (h-rh) / 2.0f, rw, rh, YELLOW);
    EndDrawing();
}
