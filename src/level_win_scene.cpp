#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>

#include "level_win_scene.h"

#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_cottagefire.h"


namespace trog {

level_win_scene::level_win_scene(session_info &sesh, bn::sprite_text_generator &text_generator, bn::random &rng) : 
        _text_generator(text_generator),
        _flames(bn::sprite_items::cottagefire.create_sprite(115, -78)),
        _burningflames(bn::create_sprite_animate_action_once(
                    _flames, 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3)),
        _trogdor(0, -5, sesh, false),
        _sesh(sesh),
        _timer(0) {

    _flames.set_visible(false);
    _flames.put_above();
    _flames.set_scale(0.7);


    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.generate(0, -60, "nice work!", _nicework_text_sprites);

    bn::string<13> word2 = "BEATEN!";
    //3% chance that the game misspells it lol
    if(rng.get_int(33) == 0){
        word2 = "BEATED!";
    }
    _trogdor.setup_win_pose();
    _trogdor.flex();
    _text_generator.generate(0, 60, "LEVEL " + word2, _levelbeated_text_sprites);

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


    if(_timer >= 50 && bn::keypad::a_pressed()){
        result = scene_type::PLAY;
    }
    
    return result;
}

}