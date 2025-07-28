#include <iostream>
#include <fstream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_video.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <nlohmann/json.hpp>

extern "C" {
    #include <lua.h>
    #include <lualib.h>
    #include <lauxlib.h>
}

using namespace std;
using nlohmann::json;

ALLEGRO_EVENT_QUEUE* event_queue = NULL;
ALLEGRO_VIDEO* video = NULL;
ALLEGRO_FONT* font = NULL;

/*
void waitSpaceC(ALLEGRO_EVENT_QUEUE* event_queue) {
    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                break;
            }
        }
    }
}
*/

int waitKeyGeneric(ALLEGRO_EVENT_QUEUE* event_queue) {
    bool key_down_received = false;

    while (true) {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            key_down_received = true;
        }
        else if (key_down_received && ev.type == ALLEGRO_EVENT_KEY_CHAR) {
            return ev.keyboard.unichar;
        }
    }
}

int clear(lua_State *L) {
    int r = luaL_checknumber(L, 1);
    int g = luaL_checknumber(L, 2);
    int b = luaL_checknumber(L, 3);

    al_clear_to_color(al_map_rgb(r, g, b));
    al_flip_display();
    
    return 1;
}

int sleep(lua_State *L) {
    double delay = luaL_checknumber(L, 1);
    
    al_rest(delay);

    return 1;
}

void videoDisplay(ALLEGRO_VIDEO *video) {
    ALLEGRO_BITMAP* frame = al_get_video_frame(video);    
    
    if (!frame) {
        return;
    }

    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_bitmap(frame, 0, 0, 0);
    al_flip_display();
    //al_destroy_bitmap(frame); //if something messes up, it's this, never tested
}

int playVid(lua_State *L) {
    const char* path = luaL_checkstring(L, 1);

    video = al_open_video(path);    

    al_start_video(video, NULL);
    al_register_event_source(event_queue, al_get_video_event_source(video));

    ALLEGRO_EVENT ev;
    while (true) {
        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
            if (ev.keyboard.keycode == ALLEGRO_KEY_SPACE) {
                break;            
            }
        } else if (ev.type == ALLEGRO_EVENT_TIMER) {
            videoDisplay(video);
        } else if (ev.type == ALLEGRO_EVENT_VIDEO_FRAME_SHOW) {
            videoDisplay(video);
        } else if (ev.type == ALLEGRO_EVENT_VIDEO_FINISHED) {
            break;
        }
    }
    
    al_close_video(video);
    
    return 1;
}

/*
int waitSpace(lua_State *L) {
    //waitForRelease(event_queue);
    waitSpaceC(event_queue);

    return 1;
}
*/

int getKey(lua_State *L) {
    //waitForRelease(event_queue);
    int ch = waitKeyGeneric(event_queue);

    char str[2];
    str[0] = (char) ch;
    str[1] = '\0';

    lua_pushstring(L, str);
    
    return 1;
}

int drawString(lua_State *L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    int r = luaL_checknumber(L, 3);
    int g = luaL_checknumber(L, 4);
    int b = luaL_checknumber(L, 5);
    const char* text = luaL_checkstring(L, 6);

    al_draw_text(font, al_map_rgb(r, g, b), x, y, ALLEGRO_ALIGN_LEFT, text);
    al_flip_display();

    return 1;
}

int drawImage(lua_State *L) {
    int x = luaL_checknumber(L, 1);
    int y = luaL_checknumber(L, 2);
    const char* path = luaL_checkstring(L, 3);

    ALLEGRO_BITMAP *image = al_load_bitmap(path);

    al_draw_bitmap(image, x, y, 0);
    al_flip_display();
    al_destroy_bitmap(image);

    return 1;
}

int setFont(lua_State *L) {
    const char* ttf = luaL_checkstring(L, 1);
    int size = luaL_checknumber(L, 2);

    font = al_load_ttf_font(ttf, size, 0);

    return 1;
}

int main() {
    ifstream input("vince.json");

    json j;
    input >> j;

    string lua_script_s = j["executing"]["script"];
    const char* lua_script = lua_script_s.c_str();
    string font_file_s = j["font"]["ttf"];
    const char* font_file = font_file_s.c_str();
    int font_size = j["font"]["size"];

    lua_State *L = luaL_newstate();
    luaL_openlibs(L);

    //lua_register(L, "WaitSpace", waitSpace);
    lua_register(L, "Clear", clear);
    lua_register(L, "Sleep", sleep);
    lua_register(L, "PlayVid", playVid);
    lua_register(L, "GetKey", getKey);
    lua_register(L, "DrawString", drawString);
    lua_register(L, "DrawImage", drawImage);
    lua_register(L, "SetFont", setFont);

    al_init();
    al_init_image_addon();
    al_init_video_addon();
    al_install_keyboard();
    al_init_ttf_addon();

    ALLEGRO_DISPLAY* display = al_create_display(640, 480);

    font = al_load_ttf_font(font_file, font_size, 0);

    event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_display_event_source(display));    
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    if (luaL_dofile(L, lua_script) != LUA_OK) {
        fprintf(stderr, "%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);    
    }

    /*
    al_clear_to_color(al_map_rgb(0, 0, 255));
    al_flip_display();

    wait_space(event_queue);

    al_clear_to_color(al_map_rgb(0, 255, 0));
    al_flip_display();

    al_rest(2);
    */    

    lua_close(L);
    al_destroy_display(display);
    
    return 0;
}
