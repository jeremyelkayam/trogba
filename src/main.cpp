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
#include "enums.h"
#include "common_stuff.h"
#include "player.h"
#include "title_scene.h"
#include "play_scene.h"
#include "gameover_scene.h"
#include "level_win_scene.h"
#include "bonus_scene.h"
#include "movie_scene.h"
#include "logo_scene.h"
#include "main_menu_scene.h"
#include "hiscores_scene.h"
#include "credits_scene.h"
#include "devs_scene.h"
#include "fireyrage_scene.h"
#include "options_scene.h"
#include "dragon_select_scene.h"
#include "cutsceneviewer_scene.h"
#include "extras_scene.h"
#include "achievements_scene.h"

int main()
{
    bn::core::init();
    bn::timer kicktimer;
    bool kicked=false;
    bn::unique_ptr<trog::scene> scene;
    bn::unique_ptr<trog::scene> previous_play_scene;
    bn::optional<trog::scene_type> next_scene = trog::scene_type::ACHIEVEMENTS;
    bn::optional<trog::scene_type> last_scene;

    
    bn::bg_palettes::set_transparent_color(bn::color(0, 0, 0));

    //doesn't *NEED* to be a unique ptr as far as code architecture goes,
    // but it's going on the heap because it takes up a lot of stack memory
    bn::unique_ptr<trog::common_stuff> common_stuff(
        new trog::common_stuff());
    
    trog::session_info sesh(*common_stuff);


    common_stuff->text_generator.set_center_alignment();

    bool logo_scene = true;

    trog::hud hud(sesh, *common_stuff, TROG_TROGMETER_MAX);
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
                    scene.reset(new trog::logo_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::TITLE: { 
                    hud.hide();
                    scene.reset(new trog::title_scene(*common_stuff));
                    break;
                }
                case trog::scene_type::MENU: { 
                    hud.hide();
                    scene.reset(new trog::main_menu_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::PLAY: { 
                    hud.show();
                    if(previous_play_scene){
                        scene = bn::move(previous_play_scene);
                    }else{
                        scene.reset(new trog::play_scene(sesh, hud, *common_stuff));
                    }
                    break;
                }
                case trog::scene_type::BONUS: {

                    previous_play_scene = bn::move(scene);

                    scene.reset(new trog::bonus_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::LOSE: { 
                    hud.show();
                    scene.reset();
                    scene.reset(new trog::gameover_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::LEVELBEAT: {
                    hud.show();
                    scene.reset(new trog::level_win_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::MOVIE: {
                    if(last_scene == trog::scene_type::LEVELBEAT){
                        hud.show();
                    }else{
                        hud.hide();
                    }
                    scene.reset(new trog::movie_scene(sesh, *common_stuff, *last_scene));
                    break;
                }
                case trog::scene_type::HISCORES: {
                    hud.hide();
                    if(last_scene == trog::scene_type::PLAY){
                        previous_play_scene = bn::move(scene);
                    }                    
                    scene.reset(new trog::hiscores_scene(sesh, *common_stuff, *last_scene));
                    break;
                }                
                case trog::scene_type::CREDITS: {
                    hud.hide();
                    scene.reset(new trog::credits_scene(*common_stuff, *last_scene));
                    break;
                }                
                case trog::scene_type::FIREYRAGE: {
                    hud.hide();
                    scene.reset(new trog::fireyrage_scene(sesh));
                    break;
                }       
                case trog::scene_type::OPTIONS: {
                    hud.hide();

                    if(last_scene == trog::scene_type::PLAY){
                        previous_play_scene = bn::move(scene);
                    }

                    scene.reset(new trog::options_scene(*common_stuff, *last_scene));
                    break;
                }                
                case trog::scene_type::DRAGON_SELECT: {
                    hud.hide();
                    scene.reset(new trog::dragon_select_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::CUTSCENE_VIEWER: {
                    hud.hide();
                    scene.reset(new trog::cutsceneviewer_scene(sesh, *common_stuff));
                    break;
                }
                case trog::scene_type::EXTRAS: {
                    hud.hide();
                    scene.reset(new trog::extras_scene(*common_stuff));
                    break;
                }
                case trog::scene_type::ACHIEVEMENTS: {
                    hud.hide();
                    scene.reset(new trog::achievements_scene(*common_stuff));
                    break;
                }
                default: { 
                    BN_ERROR("the selected screen does not exist or is not yet implemented");
                    break;
                }
            }
            last_scene = next_scene;
        }

        if(!kicked && bn::keypad::select_pressed()){
            BN_LOG("kick. TROGADOR");
            bn::sound_items::kick.play(common_stuff->savefile.options.sound_vol);
            kicktimer.restart();
            kicked=true;
        }
        if(kicked && kicktimer.elapsed_ticks() > 120000) { 
            bn::sound_items::trogador.play(common_stuff->savefile.options.sound_vol);
            kicked=false;
        }

        // soft reset code
        if(bn::keypad::start_held() && bn::keypad::select_held() &&
                bn::keypad::a_held() && bn::keypad::b_held() && !logo_scene){
            scene.reset();
            bn::core::reset();
        }

        // Burn a random number every frame.
        // This makes it less likely to get the same random numbers every time you play
        common_stuff->rand.update();
        common_stuff->commentary.update();
        common_stuff->acm.update();

        //increment our total play time by 1 frame... 
        common_stuff->savefile.stats.play_time++;

        bn::core::update();
    }
}

