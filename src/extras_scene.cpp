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
#include "bn_regular_bg_items_main_menu.h"

#define SELECTION_ANIM_LENGTH 30

namespace trog {

extras_scene::extras_scene(session_info &sesh, common_stuff &common_stuff) : 
        menu_scene(common_stuff, bn::regular_bg_items::main_menu) {

    common_stuff.text_generator.set_center_alignment();

    // _menu_options.emplace_back( "HOW TO PLAY", common_stuff.text_generator);

    // _menu_options.emplace_back(5, 77, "HI-SCORES", common_stuff.text_generator);
    // _menu_options.emplace_back(5, 107, "CREDITS", common_stuff.text_generator);
    // _menu_options.emplace_back(5, 137, "CUTSCENE VIEWER", common_stuff.text_generator);
}

bn::optional<scene_type> extras_scene::update(){

    bn::optional<scene_type> result = menu_scene::update();

    if(bn::keypad::a_pressed()){

        result = scene_type::PLAY;


    }else if((bn::keypad::up_pressed() || bn::keypad::down_pressed())){


    }else if((bn::keypad::left_pressed())){
        if(_selected_option_index > 1){
            --_selected_option_index;
        }
    }else if((bn::keypad::right_pressed())){
        if(_selected_option_index != 0 && _selected_option_index < 3){
            ++_selected_option_index;
        }
    }

    return result;
}

}