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
#include <bn_timer.h>
#include "player.h"
#include "title_scene.h"
#include "scene_type.h"

//debug settings for emulator
#define BN_LOG_BACKEND_MGBA true

int main()
{
    bn::core::init();
    bn::timer kicktimer;
    bool kicked=false;
    // trog::player tragdar = trog::player();
    // bn::timer looptimer;
    bn::unique_ptr<trog::scene> scene;
    bn::optional<trog::scene_type> next_scene = trog::scene_type::TITLE;

    while(true)
    {
        if(scene){
            next_scene = scene->update();
        }
        if(next_scene){
            switch(*next_scene){
                case trog::scene_type::TITLE: { 
                    scene.reset(new trog::title_scene());
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

// void debug_adjust(bn::regular_bg_ptr bg){
//     //debug adjustment -- move a background around with the dpad and log the coords
//     if(bn::keypad::left_held()){
//         bg.set_x(bg.x() - 1);
//         BN_LOG("xcor ", bg.x());
//     }
//     else if(bn::keypad::right_held()){
//         bg.set_x(bg.x() + 1);
//         BN_LOG("xcor ", bg.x());
//     }
//     if(bn::keypad::up_held()){
//         bg.set_y(bg.y() - 1);
//         BN_LOG("ycor", bg.y());
//     }
//     else if(bn::keypad::down_held()){
//         bg.set_y(bg.y() + 1);
//         BN_LOG("ycor", bg.y());
//     }
// }