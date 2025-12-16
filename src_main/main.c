#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "vdp.h"
#include "input.h"
#include "scene.h"
#include "comm_main.h"
#include "save.h"

static scene_id_t scene = SCENE_TITLE;
static int menu_index = 0;
static int pause_index = 0;
static int tech_progress = 0;
static char tech_buffer[4096] __attribute__((aligned(2)));
static pad_state_t pad;
static char save_status[32] = "READY";

static void delay(void) {
    for (volatile int i = 0; i < 4000; ++i) {}
}

static void draw_title(void) {
    vdp_clear();
    vdp_puts(10, 10, "MINNKA");
    vdp_puts(8, 14, "Press START");
}

static void draw_menu(void) {
    vdp_clear();
    vdp_puts(8, 6, "MAIN MENU");
    const char *items[] = {"PLAY", "TECH DEMO", "SAVE TEST", "ABOUT"};
    for (int i = 0; i < 4; ++i) {
        vdp_puts(6, 10 + i*2, i == menu_index ? ">" : " ");
        vdp_puts(8, 10 + i*2, items[i]);
    }
}

static void draw_pause(void) {
    vdp_puts(8, 22, "PAUSE");
    const char *items[] = {"RESUME", "RETURN"};
    for (int i = 0; i < 2; ++i) {
        vdp_puts(6, 24 + i, i == pause_index ? ">" : " ");
        vdp_puts(8, 24 + i, items[i]);
    }
}

static void draw_about(void) {
    vdp_clear();
    vdp_puts(6, 6, "ABOUT");
    vdp_puts(4, 9, "Build: Mega-CD starter");
    vdp_puts(4, 11, "Controls: D-PAD, A/B/C, START");
    vdp_puts(4, 13, "Press B to return");
}

static void draw_save(void) {
    vdp_clear();
    vdp_puts(6, 6, "SAVE TEST");
    const save_data_t *s = save_get();
    char line[40];
    for (int i = 0; i < 3; ++i) {
        snprintf(line, sizeof(line), "Slot%d magic:%04X", i, (unsigned int)s->magic);
        vdp_puts(4, 10 + i*2, line);
    }
    snprintf(line, sizeof(line), "Status: %s", save_status);
    vdp_puts(4, 16, line);
    vdp_puts(4, 18, "A=WRITE  B=READ  START=BACK");
}

static void draw_game(int px, int py) {
    vdp_clear();
    vdp_puts(2,2,"LEVEL 1");
    vdp_puts(px/8, py/8, "@");
    draw_pause();
}

static void draw_tech(void) {
    vdp_clear();
    vdp_puts(6, 6, "TECH DEMO");
    vdp_puts(4, 10, "Loading DATA/TEST.BIN");
    char buf[32];
    snprintf(buf, sizeof(buf), "Progress: %d", tech_progress);
    vdp_puts(4, 12, buf);
    vdp_puts(4, 14, "Palette cycle running");
    vdp_puts(4, 16, "START toggles PCM stub");
}

static void apply_palette_cycle(void) {
    static uint16_t base = 0x000;
    base = (base + 1) & 0xEEE;
    for (int i = 0; i < 16; ++i) {
        vdp_set_palette(i, base ^ (i<<1));
    }
}

static void handle_title(void) {
    if (pad.start || pad.a || pad.b || pad.c) {
        scene = SCENE_MENU;
        menu_index = 0;
        draw_menu();
    }
}

static void handle_menu(void) {
    if (pad.down) { if (menu_index < 3) menu_index++; draw_menu(); }
    if (pad.up) { if (menu_index > 0) menu_index--; draw_menu(); }
    if (pad.start || pad.a || pad.c) {
        switch(menu_index) {
            case 0: scene = SCENE_PLAY; break;
            case 1: scene = SCENE_TECH; break;
            case 2: scene = SCENE_SAVE; break;
            case 3: scene = SCENE_ABOUT; break;
        }
    }
}

static void handle_about(void) {
    if (pad.b || pad.start) {
        scene = SCENE_MENU;
        draw_menu();
    }
}

static void handle_save(void) {
    if (pad.start) { scene = SCENE_MENU; draw_menu(); return; }
    if (pad.a) {
        uint8_t payload[16];
        for (int i = 0; i < 16; ++i) payload[i] = (uint8_t)(tech_progress + i);
        save_write_slot(0, payload, sizeof(payload));
        strncpy(save_status, "WROTE SLOT0", sizeof(save_status)-1);
        draw_save();
    }
    if (pad.b) {
        if (save_verify(save_get())) {
            strncpy(save_status, "VERIFY OK", sizeof(save_status)-1);
        } else {
            strncpy(save_status, "BAD CHECKSUM", sizeof(save_status)-1);
        }
        draw_save();
    }
}

static void handle_pause(void) {
    if (pad.down) { pause_index = (pause_index+1)%2; draw_pause(); }
    if (pad.up) { pause_index = (pause_index+1)%2; draw_pause(); }
    if (pad.a || pad.start) {
        if (pause_index==0) scene = SCENE_PLAY; else { scene = SCENE_TITLE; draw_title(); }
    }
}

static void handle_game(void) {
    static int px = 40, py = 120;
    static int vy = 0;
    if (pad.start) { scene = SCENE_PAUSE; pause_index=0; draw_pause(); return; }
    if (pad.left) px -= 2;
    if (pad.right) px += 2;
    vy += 1;
    if (pad.a && py >= 160) vy = -10;
    py += vy;
    if (py > 160) { py = 160; vy = 0; }
    draw_game(px, py);
}

static void handle_tech(void) {
    static int pcm_on = 0;
    tech_progress++;
    apply_palette_cycle();
    if (tech_progress % 60 == 1) {
        uint32_t read = 0;
        comm_load_file("DATA/TEST.BIN", tech_buffer, sizeof(tech_buffer), &read);
    }
    if (pad.start) {
        pcm_on = !pcm_on;
        if (pcm_on) comm_pcm_start(); else comm_pcm_stop();
    }
    if (pad.b) { scene = SCENE_MENU; draw_menu(); }
}

int main(void) {
    vdp_init();
    vdp_load_font();
    input_init();
    comm_init();
    save_init();
    draw_title();
    while (1) {
        input_read(&pad);
        switch(scene) {
            case SCENE_TITLE: handle_title(); break;
            case SCENE_MENU: handle_menu(); break;
            case SCENE_PLAY: handle_game(); break;
            case SCENE_TECH: handle_tech(); break;
            case SCENE_SAVE: handle_save(); break;
            case SCENE_ABOUT: handle_about(); break;
            case SCENE_PAUSE: handle_pause(); break;
        }
        delay();
    }
    return 0;
}
