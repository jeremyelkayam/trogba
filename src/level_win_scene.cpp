#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_sram.h>
#include "level_win_scene.h"

#include "bn_regular_bg_items_trogsmile.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_nose_smoke.h"
#include "bn_sprite_items_cottagefire.h"


namespace trog {

level_win_scene::level_win_scene(session_info &sesh, bn::sprite_text_generator &text_generator) : 
        _happy_trogdor(bn::regular_bg_items::trogsmile.create_bg(8, 61)),
        _text_generator(text_generator),
        _nose_smoke(bn::sprite_items::nose_smoke.create_sprite(40, -35)),
        _flames(bn::sprite_items::cottagefire.create_sprite(115, -78)),
        _smoke_anim(bn::create_sprite_animate_action_once(
                    _nose_smoke, 3, bn::sprite_items::nose_smoke.tiles_item(),
                    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16,
                    17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30)),
        _burningflames(bn::create_sprite_animate_action_once(
                    _flames, 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3)),
        _sesh(sesh),
        _timer(0) {

    _flames.set_visible(false);
    _flames.put_above();
    _flames.set_scale(0.7);

    bn::sound_items::burninate.play(TROG_DEFAULT_VOLUME);

    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.generate(73, 5, "nice work!", _nicework_text_sprites);

    bn::string<7> line2 = "BEATEN!";
    //3% chance that the game misspells it lol
    if(rand() % 33 == 0){
        line2 = "BEATED!";
    }
    _text_generator.generate(70, 40, "LEVEL", _levelbeated_text_sprites);
    _text_generator.generate(70, 55, line2, _levelbeated_text_sprites);

    bn::sram::write(sesh);
}

bn::optional<scene_type> level_win_scene::update(){
    ++_timer;
    bn::optional<scene_type> result;

    if(30 < _timer && !_burningflames.done()){
        _flames.set_visible(true);
        _burningflames.update();
    }
    if(_timer == 40) { 
        _sesh.advance_level();
    }
    if(_burningflames.done()){
        _flames.set_visible(false);
    }

    if(!_smoke_anim.done()){
        _smoke_anim.update();
    }else{
        _nose_smoke.set_visible(false);
        if(bn::keypad::a_pressed()){
            if(_sesh.current_level_has_cutscene()) {
                result = scene_type::MOVIE;
            }else{
                result = scene_type::PLAY;
            }
        }

    }

    return result;
}

}