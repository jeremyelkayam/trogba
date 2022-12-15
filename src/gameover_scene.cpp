#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "gameover_scene.h"

#include "bn_regular_bg_items_trogdead.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_jonathan_howe.h"


namespace trog {

gameover_scene::gameover_scene(session_info &sesh, bn::sprite_text_generator &text_generator) : 
        _dead_trogdor(bn::regular_bg_items::trogdead.create_bg(TROG_GAMEOVER_BG_X, TROG_GAMEOVER_BG_Y)),
        _sesh(sesh),
        _itsover_text(false, TROG_GAMEOVER_BIGTEXT_X, TROG_GAMEOVER_BIGTEXT_Y, "IT'S OVER!",
            bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item()) {

    bn::sound_items::gameover.play(TROG_DEFAULT_VOLUME);

    if(_sesh.get_score() >= 3000){
        for(int z = 0; z < 3; ++z){
            //Jonathan Howe name easter egg
            //todo: maybe add my name to it since i madethis ??? 
            _secret_sprites.emplace_back(bn::sprite_items::jonathan_howe.create_sprite(
                TROG_GAMEOVER_SECRET_TEXT_X + 32*z, TROG_GAMEOVER_SECRET_TEXT_Y, z*6));
            _secret_anims.emplace_back(bn::create_sprite_animate_action_forever(
                    _secret_sprites.at(z), 3, bn::sprite_items::jonathan_howe.tiles_item(), 
                    0 +6*z, 1 +6*z, 2 +6*z, 3 +6*z, 4+6*z, 5+6*z)
            );
        }
    }
}

bn::optional<scene_type> gameover_scene::update(){

    for(auto &anim : _secret_anims) {
        anim.update();
    }

    _itsover_text.update();
    
    bn::optional<scene_type> result;
    
    if(bn::keypad::a_pressed()){
        _sesh.reset();
        result = scene_type::PLAY;
    }

    return result;
}

}