#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_sram.h>
#include <bn_blending.h>
#include <bn_sprite_palettes.h>
#include <bn_bg_palettes.h>

#include "gameover_scene.h"
#include "sb_commentary.h"

#include "bn_regular_bg_items_trogdead.h"
#include "bn_regular_bg_items_game_over_menu.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_jonathan_howe.h"
#include "bn_sprite_items_jeremy_elkayam.h"


namespace trog {

gameover_scene::gameover_scene(session_info &sesh, common_stuff &common_stuff) : 
        _dead_trogdor(bn::regular_bg_items::trogdead.create_bg(TROG_GAMEOVER_BG_X, TROG_GAMEOVER_BG_Y)),
        _sesh(sesh),
        _itsover_text(false, TROG_GAMEOVER_BIGTEXT_X, TROG_GAMEOVER_BIGTEXT_Y, "IT'S OVER!",
            bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item(), common_stuff.rand),
        _common_stuff(common_stuff),
        _menu_option(0) {

    if(_sesh.get_score() >= TROG_GAMEOVER_SECRET_SCORE){

        bn::sprite_item item = _sesh.get_score() >= TROG_GAMEOVER_SUPER_SECRET_SCORE ? 
              bn::sprite_items::jeremy_elkayam
            : bn::sprite_items::jonathan_howe;
        for(int z = 0; z < 3; ++z){
            _secret_sprites.emplace_back(item.create_sprite(
                TROG_GAMEOVER_SECRET_TEXT_X + 32*z, TROG_GAMEOVER_SECRET_TEXT_Y, z*6));
            _secret_anims.emplace_back(bn::create_sprite_animate_action_forever(
                    _secret_sprites.at(z), 3, item.tiles_item(), 
                    0 +6*z, 1 +6*z, 2 +6*z, 3 +6*z, 4+6*z, 5+6*z)
            );
        }
    }

    //todo: refactor this
    if(_sesh.last_killed_by_archer()){
        _common_stuff.commentary.gameover_arch();
        bn::sound_items::gameover.play(common_stuff.savefile.sound_vol * bn::fixed(0.2));
    }else if(_common_stuff.commentary.gameover(_sesh.get_score()))
    {
        bn::sound_items::gameover.play(common_stuff.savefile.sound_vol * bn::fixed(0.2));
    }else{
        bn::sound_items::gameover.play(common_stuff.savefile.sound_vol);
    }

    common_stuff.text_generator.set_center_alignment();
    common_stuff.text_generator.set_palette_item(RED_PALETTE);
    common_stuff.text_generator.generate(1, -49, "CHALLENGE", _challengeagain_text_sprites);
    common_stuff.text_generator.generate(0, -36, "AGAIN!", _challengeagain_text_sprites);

    common_stuff.text_generator.generate(0, -4, "VIEW", _hiscores_text_sprites);
    common_stuff.text_generator.generate(2, 9, "HI-SCORES", _hiscores_text_sprites);

    common_stuff.text_generator.generate(0, 50, "BACK", _back_text_sprites);

    common_stuff.set_sprite_arr_visible(_challengeagain_text_sprites, false);
    common_stuff.set_sprite_arr_visible(_hiscores_text_sprites, false);
    common_stuff.set_sprite_arr_visible(_back_text_sprites, false);
}


bn::optional<scene_type> gameover_scene::update(){

    for(auto &anim : _secret_anims) {
        anim.update();
    }

    _itsover_text.update();
    
    bn::optional<scene_type> result;


    if(_menu){
        if(bn::keypad::up_pressed()){
            if(_menu_option == 0){
                _menu_option = 2;
            }else{
                --_menu_option;
            }
            set_current_menu_option_visible();
        }else if(bn::keypad::down_pressed()){
            if(_menu_option == 2){
                _menu_option = 0;
            }else{
                ++_menu_option;
            }
            set_current_menu_option_visible();
        }
        
    }

    if(bn::keypad::a_pressed()){
        if(!_menu){
            _menu = bn::regular_bg_items::game_over_menu.create_bg(TROG_GAMEOVER_MENU_X, TROG_GAMEOVER_MENU_Y);
            _menu->set_priority(1);
            bn::blending::set_transparency_alpha(0.5);
            _dead_trogdor.set_blending_enabled(true);
            for(bn::sprite_ptr &sprite : _secret_sprites) {
                sprite.set_blending_enabled(true);
            }
            _itsover_text.set_blending_enabled(true);
            set_current_menu_option_visible();
        }else{
            switch(_menu_option){
                case 0:
                    _sesh.reset();
                    result = scene_type::PLAY;
                    break;
                case 1:
                    result = scene_type::HISCORES;
                    break;
                case 2:
                    _sesh.reset();
                    result = scene_type::MENU;
                    break;
                default:
                break;                
            }
        }
    }

    if(result){
        
        _menu.reset();
    }

    return result;
}

void gameover_scene::set_current_menu_option_visible(){
    _common_stuff.set_sprite_arr_visible(_challengeagain_text_sprites, _menu_option == 0);
    _common_stuff.set_sprite_arr_visible(_hiscores_text_sprites, _menu_option == 1);
    _common_stuff.set_sprite_arr_visible(_back_text_sprites, _menu_option == 2);
}

}