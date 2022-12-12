#include <bn_keypad.h>
#include "movie_scene.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_strongbad.h"
#include "bn_sprite_items_player.h"

namespace trog{

movie_scene::movie_scene(session_info &sesh, bn::sprite_text_generator &text_generator) : 
    _character_sprite(bn::sprite_items::strongbad.create_sprite(130, 0)),
    _sb_anim(bn::create_sprite_animate_action_forever(
                _character_sprite, 5, bn::sprite_items::strongbad.tiles_item(),
                0, 1 )),
    _sb_move(_character_sprite, 180, 0, 0),
    _timer(0),
    _text_generator(text_generator),
    _sesh(sesh)
{

}

bn::optional<scene_type> movie_scene::update(){
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.set_center_alignment();

    if(_sesh.get_level() == 101) { 
        update_ending_cutscene();

    }

    bn::optional<scene_type> result;
    

    // move_background(_dead_trogdor);

    return result;
}

bool movie_scene::cutscene_over(){
    return false;
}

void movie_scene::update_ending_cutscene(){
    if(!_sb_move.done()){
        _sb_anim.update();
        _sb_move.update();
    }else{
        if(_timer == 0){
            _sb_anim = bn::create_sprite_animate_action_forever(
                    _character_sprite, 10, bn::sprite_items::strongbad.tiles_item(),
                    2, 3 );            
            _sb_anim.update();
        }
        if(_timer == 30){
            bn::sound_items::cutscene_congrats.play(TROG_DEFAULT_VOLUME);
            _text_generator.generate(0, -60, "congratulations.", _text_sprites);
        }
        if(_timer == 130){
            _text_generator.generate(0, -48, "you got", _text_sprites);
        }
        if(_timer == 190){
            _text_generator.generate(0, -36, "good score", _text_sprites);
        }
        if(_timer > 30 && _timer < 210){
            _sb_anim.update();
        }        

        if(_timer == 240){
            bn::sound_items::cutscene_credits.play(TROG_DEFAULT_VOLUME);
            _text_sprites.clear();
            _text_generator.generate(0, -60, "keep playing!", _text_sprites);
            _character_sprite.set_x(30);
            _character_sprite.set_tiles(bn::sprite_items::strongbad.tiles_item(), 3);
            _character_sprite.set_rotation_angle(315); //bn::sprite_items::player.create_sprite(0, 0);
            _character_sprite = bn::sprite_items::player.create_sprite(-30, 0);
            _character_sprite.set_rotation_angle(45); //bn::sprite_items::player.create_sprite(0, 0);

        }
        _timer++;
    }
}

}