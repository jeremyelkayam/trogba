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
#include "trogdor_variable_8x16_sprite_font.h"
#include "trogdor_variable_32x64_sprite_font.h"
#include "trogdor_variable_8x8_sprite_font.h"
#include "player.h"
#include "title_scene.h"
// #include "instructions_scene.h"
#include "play_scene.h"
#include "gameover_scene.h"
#include "level_win_scene.h"
#include "bonus_scene.h"
#include "movie_scene.h"
#include "logo_scene.h"
#include "menu_scene.h"
#include "hiscores_scene.h"
#include "credits_scene.h"
#include "devs_scene.h"
#include "fireyrage_scene.h"
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
    bn::optional<trog::scene_type> next_scene = trog::scene_type::LOGO;
    trog::session_info sesh;
    
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));
    bn::sprite_text_generator text_generator(trog::variable_8x16_sprite_font);
    bn::sprite_text_generator big_text_generator(trog::variable_32x64_sprite_font);
    bn::sprite_text_generator small_text_generator(trog::variable_8x8_sprite_font);
    bn::random rand;
    text_generator.set_center_alignment();

    bool logo_scene = true;

    trog::hud hud(sesh, text_generator, TROG_TROGMETER_MAX);
    hud.hide();


    while(true)
    {
        if(scene){
            next_scene = scene->update();
            hud.update();
        }
        if(next_scene){
            bn::sound::stop_all();
            logo_scene = (*next_scene == trog::scene_type::LOGO);
            switch(*next_scene){
                case trog::scene_type::DEVS: { 
                    hud.hide();
                    scene.reset(new trog::devs_scene());
                    break;
                }
                case trog::scene_type::LOGO: { 
                    hud.hide();
                    scene.reset(new trog::logo_scene(sesh));
                    break;
                }
                case trog::scene_type::TITLE: { 
                    hud.hide();
                    scene.reset(new trog::title_scene());
                    break;
                }
                // instructions scene has been replaced by menu scene.
                // case trog::scene_type::INSTRUCTIONS: { 
                //     hud.hide();
                //     scene.reset(new trog::instructions_scene(sesh, text_generator));
                //     break;
                // }
                case trog::scene_type::MENU: { 
                    hud.hide();
                    scene.reset(new trog::menu_scene(sesh, text_generator, small_text_generator));
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
                        scene.reset(new trog::play_scene(sesh, hud, text_generator, small_text_generator, rand));
                    }
                    break;
                }
                case trog::scene_type::BONUS: {
                    // hud.show();
                    //this is the treasure hut scene
                    // we need to store the play scene so that we may return to it later.
                    previous_play_scene = bn::move(scene);

                    scene.reset(new trog::bonus_scene(sesh));
                    break;
                }
                case trog::scene_type::LOSE: { 
                    hud.show();
                    scene.reset();
                    scene.reset(new trog::gameover_scene(sesh, text_generator, rand));
                    break;
                }
                case trog::scene_type::LEVELBEAT: {
                    hud.show();
                    scene.reset(new trog::level_win_scene(sesh, text_generator, small_text_generator, rand));
                    break;
                }
                case trog::scene_type::MOVIE: {
                    hud.show();
                    scene.reset(new trog::movie_scene(sesh, text_generator, rand));
                    break;
                }
                case trog::scene_type::HISCORES: {
                    hud.hide();
                    scene.reset(new trog::hiscores_scene(sesh));
                    break;
                }                
                case trog::scene_type::CREDITS: {
                    hud.hide();
                    scene.reset(new trog::credits_scene(text_generator));
                    break;
                }                
                case trog::scene_type::FIREYRAGE: {
                    hud.hide();
                    scene.reset(new trog::fireyrage_scene(sesh));
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
            bn::sound_items::kick.play(TROG_DEFAULT_VOLUME);
            kicktimer.restart();
            kicked=true;
        }
        if(kicked && kicktimer.elapsed_ticks() > 120000) { 
            bn::sound_items::trogador.play(TROG_DEFAULT_VOLUME);
            kicked=false;
        }

        // soft reset code
        if(bn::keypad::start_held() && bn::keypad::select_held() &&
                bn::keypad::a_held() && bn::keypad::b_held() && !logo_scene){
            bn::core::reset();
        }

        // Burn a random number every frame.
        // This makes it less likely to get the same random numbers every time you play
        rand.update();

        bn::core::update();
    }
}

