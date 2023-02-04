#include <bn_sram.h>
#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_log.h>

#include "menu_scene.h"
#include "bn_sprite_items_arrow.h"

namespace trog {

menu_scene::menu_scene(session_info &sesh, bn::sprite_text_generator& text_generator) : 
        _text_generator(text_generator),
        _cursor(bn::sprite_items::arrow.create_sprite(-110,-30)),
        _selected_option_index(0),
        _sesh(sesh) {

            

    bn::sram::read(_loaded_sesh);

    if(_loaded_sesh.is_valid_object()){
        _text_generator.set_left_alignment();
        _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
        _text_generator.generate(-100, -30, "CONTINUE", _menu_text_sprites);    

        bn::string<64> session_summary;
        bn::ostringstream summary_stream(session_summary);
        summary_stream << "score: " << _loaded_sesh.get_score() << "  ";       
        summary_stream << "mans: " << _loaded_sesh.get_mans() << "  ";       
        summary_stream << "level: " << _loaded_sesh.get_level();       

        _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        _text_generator.generate(-90, -15, session_summary, _menu_text_sprites);
        _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }

    _text_generator.generate(-100, 10, "NEW GAME", _menu_text_sprites);
}

bn::optional<scene_type> menu_scene::update(){
    bn::optional<scene_type> result;
    if(bn::keypad::a_pressed()){
        if(_selected_option_index == 0){
            //load file
            if(_loaded_sesh.is_valid_object()){
                _sesh = _loaded_sesh;
                bn::sram::clear(sizeof(_sesh));
                BN_LOG("loaded the file");
            }else{
                // probably provide some type of WRONG sound effect to provide feedback
            }
        }else if(_selected_option_index == 1){
            //new game
        }
        result = scene_type::PLAY;

    }else if(bn::keypad::up_pressed()){
        --_selected_option_index;
    }else if(bn::keypad::down_pressed()){
        ++_selected_option_index;
    }

    if(bn::keypad::any_pressed()){
        _selected_option_index = _selected_option_index % 2;
        _cursor.set_y(-30 + _selected_option_index * 40);
    }

    return result;
}

}