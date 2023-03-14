/*
 * main.cpp
 * Purpose: run the game trogdor on a gba
 * 
 * Libraries: Butano, by Gustavo Valiente
 */

#include <bn_core.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sound.h>
#include <bn_timer.h>
#include <bn_bg_palettes.h>
#include <bn_sprite_text_generator.h>
#include "trogdor_variable_8x16_sprite_font.h"
#include "player.h"
#include "instructions_scene.h"
#include "play_scene.h"
#include "level_win_scene.h"
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
    bn::unique_ptr<trog::scene> previous_play_scene;
    bn::optional<trog::scene_type> next_scene = trog::scene_type::INSTRUCTIONS;
    trog::session_info sesh;
    
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    bn::sprite_text_generator text_generator(trog::variable_8x16_sprite_font);
    text_generator.set_center_alignment();

    trog::hud hud(sesh, text_generator, 10);
    hud.hide();


    while(true)
    {
        if(scene){
            next_scene = scene->update();
            hud.update();
        }
        if(next_scene){
            bn::sound::stop_all();
            switch(*next_scene){
                case trog::scene_type::INSTRUCTIONS: { 
                    hud.hide();
                    scene.reset(new trog::instructions_scene(sesh, text_generator));
                    break;
                }
                case trog::scene_type::PLAY: { 
                    hud.show();
                    if(previous_play_scene){
                        //this means we are returning from an interruption
                        //so we should go back to the play scene from before 
                        BN_LOG("returning from treasure hut");
                        scene = bn::move(previous_play_scene);
                    }else{
                        scene.reset(new trog::play_scene(sesh, hud, text_generator));
                    }
                    break;
                }
                case trog::scene_type::LEVELBEAT: {
                    hud.show();
                    scene.reset(new trog::level_win_scene(sesh, text_generator));
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
            kicktimer.restart();
            kicked=true;
        }
        if(kicked && kicktimer.elapsed_ticks() > 120000) { 
            kicked=false;
        }

        // soft reset code
        if(bn::keypad::start_held() && bn::keypad::select_held() &&
                bn::keypad::a_held() && bn::keypad::b_held() ){
            bn::core::reset();
        }

        // Burn a random number every frame.
        // This makes it less likely to get the same random numbers every time you play
        rand();
        bn::core::update();
    }
}

