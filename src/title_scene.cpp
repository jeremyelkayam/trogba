#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "bn_regular_bg_items_titlebg.h"
#include "bn_regular_bg_items_titlegraphic.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "title_scene.h"

namespace trog {

title_scene::title_scene(common_stuff &common_stuff) : 
        _common_stuff(common_stuff),
        _titlebg(bn::regular_bg_items::titlebg.create_bg(TROG_TITLE_BG_X, TROG_TITLE_BG_Y)),
        _titlegraphic(bn::regular_bg_items::titlegraphic.create_bg(0, TROG_TITLE_TEXT_Y)),
        _frame_counter(0),
        _secret_code_index(0) {
    _common_stuff.small_generator.set_left_alignment();
    _common_stuff.small_generator.set_palette_item(WHITE_PALETTE);
    _common_stuff.small_generator.generate(-120, 77, bn::string<6>("v") + TROG_SEMANTIC_VERSION_NUMBER, _version_label);

    bn::sound_items::themesong.play(common_stuff.savefile.options.music_vol);

    _secret_code.emplace_back(bn::keypad::key_type::UP);
    _secret_code.emplace_back(bn::keypad::key_type::UP);
    _secret_code.emplace_back(bn::keypad::key_type::DOWN);
    _secret_code.emplace_back(bn::keypad::key_type::DOWN);
    _secret_code.emplace_back(bn::keypad::key_type::LEFT);
    _secret_code.emplace_back(bn::keypad::key_type::RIGHT);
    _secret_code.emplace_back(bn::keypad::key_type::LEFT);
    _secret_code.emplace_back(bn::keypad::key_type::RIGHT);
    _secret_code.emplace_back(bn::keypad::key_type::B);
    _secret_code.emplace_back(bn::keypad::key_type::A);
}

bn::optional<scene_type> title_scene::update(){
    bn::optional<scene_type> result;
    _frame_counter++;

    if(bn::keypad::any_pressed() && _secret_code_index < _secret_code.size()){
        if(bn::keypad::pressed(_secret_code.at(_secret_code_index))){
            ++_secret_code_index;
        }else{
            _secret_code_index=0;
        }
    }

    if(_frame_counter > 440 || bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        result = scene_type::MENU;
        if(_secret_code_index == _secret_code.size()){
            //TODO Change to achievement
            // _common_stuff.savefile.cheat_unlocked = true;
            _common_stuff.savefile.options.starting_lives = 30;
            _common_stuff.save();
        }
    }


    return result;
}

}