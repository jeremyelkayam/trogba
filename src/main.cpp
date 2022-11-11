/*
 * main.cpp
 * Purpose: run the game trogdor on a gba
 * 
 * Libraries: Butano, by Gustavo Valiente
 */

#include <bn_core.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_sound.h>
#include <bn_timer.h>
#include <bn_bg_palettes.h>
#include <bn_sprite_text_generator.h>
#include "common_variable_8x8_sprite_font.h"
#include "player.h"
#include "title_scene.h"
#include "instructions_scene.h"
#include "play_scene.h"
#include "enums.h"

//debug settings for emulator
#define BN_LOG_BACKEND_MGBA true

int main()
{
    bn::core::init();
    bn::timer kicktimer;
    bool kicked=false;
    // bn::timer looptimer;
    bn::unique_ptr<trog::scene> scene;
    bn::optional<trog::scene_type> next_scene = trog::scene_type::TITLE;
    trog::session_info sesh = {TROG_STARTING_LIVES, 10000, TROG_STARTING_LEVEL};
    
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    bn::sprite_text_generator text_generator(common::variable_8x8_sprite_font);
    text_generator.set_center_alignment();


    while(true)
    {
        if(scene){
            next_scene = scene->update();
        }
        if(next_scene){
            bn::sound::stop_all();
            switch(*next_scene){
                case trog::scene_type::TITLE: { 
                    scene.reset(new trog::title_scene());
                    break;
                }
                case trog::scene_type::INSTRUCTIONS: { 
                    scene.reset(new trog::instructions_scene(text_generator));
                    break;
                }
                case trog::scene_type::PLAY: { 
                    scene.reset(new trog::play_scene(sesh, text_generator));
                    break;
                }                
                default: { 
                    BN_ERROR("the selected screen does not exist or is not yet implemented");
                    break;
                }
            }
        }

        if(!kicked && bn::keypad::select_pressed()){
            BN_LOG("kick. TROGADOR");
            bn::sound_items::kick.play(1);
            kicktimer.restart();
            kicked=true;
        }
        if(kicked && kicktimer.elapsed_ticks() > 120000) { 
            bn::sound_items::trogador.play(1);
            kicked=false;
        }
        // tragdar.update();
        // BN_LOG("ticks elapsed: ", looptimer.elapsed_ticks());
        // looptimer.restart();
        bn::core::update();
    }
}

