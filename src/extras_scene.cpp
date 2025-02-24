#include "extras_scene.h"

#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sound_items.h>

#include "main_menu_scene.h"
#include "bn_sprite_items_trogdorhead.h"
#include "bn_sprite_items_firebreath.h"
#include "bn_sprite_items_troghammer_icon.h"
#include "bn_sprite_items_trogmeter_degrade_icon.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_regular_bg_items_extras_menu_bg.h"

#define SELECTION_ANIM_LENGTH 30

namespace trog {

extras_scene::extras_scene(common_stuff &common_stuff) : 
        menu_scene(common_stuff, bn::regular_bg_items::extras_menu_bg) {

    common_stuff.text_generator.set_center_alignment();

    _menu_options.emplace_back(-56, -40, "ACHIEVE-\nMENTS", 
        common_stuff.text_generator, scene_type::ACHIEVEMENTS);

    _menu_options.emplace_back(56, -40, "HI-SCORES", 
        common_stuff.text_generator, scene_type::HISCORES);
    _menu_options.emplace_back(-56, 35, "CREDITS", 
        common_stuff.text_generator, scene_type::CREDITS);
    _menu_options.emplace_back(56, 35, "CUTSCENE\nVIEWER", 
        common_stuff.text_generator, scene_type::CUTSCENE_VIEWER);
}

bn::optional<scene_type> extras_scene::update(){

    bn::optional<scene_type> result = menu_scene::update();

    menu_option &current_option = _menu_options.at(_selected_option_index);

    if(bn::keypad::a_pressed()){
        result = current_option.next_scene();

    }else if(bn::keypad::up_pressed()){
        _selected_option_index -= 2;
        if(_selected_option_index < 0) _selected_option_index +=2;
    }else if(bn::keypad::down_pressed()){
        _selected_option_index += 2;
        if(_selected_option_index > 3) _selected_option_index -=2;
    }else if(bn::keypad::left_pressed() && _selected_option_index != 2){
        _selected_option_index -= 1;
        if(_selected_option_index < 0) _selected_option_index +=1;
    }else if(bn::keypad::right_pressed() && _selected_option_index != 1){
        _selected_option_index += 1;
        if(_selected_option_index > 3) _selected_option_index -=1;
    }

    if(bn::keypad::b_pressed())
    {
        result = scene_type::MENU;
    }


    return result;
}

}