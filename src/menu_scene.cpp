#include <bn_sram.h>
#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_log.h>

#include "menu_scene.h"
#include "bn_sprite_items_trogdorhead.h"
#include "bn_sprite_items_firebreath.h"

namespace trog {

menu_scene::menu_scene(session_info &sesh, bn::sprite_text_generator& text_generator, bn::sprite_text_generator &small_generator) : 
        _text_generator(text_generator),
        // _small_text_generator(variable_8x8_sprite_font),
        _cursor(bn::sprite_items::trogdorhead.create_sprite(-110,-30)),
        _flames(bn::sprite_items::firebreath.create_sprite(-110,-30)),
        _flames_anim(bn::create_sprite_animate_action_forever(
                    _flames, 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3)),
        _flames_scale(_flames, 30, 4),
        _selected_option_index(0),
        _selection_anim_timer(0),
        _sesh(sesh) {


    _flames.set_visible(false);
    _flames.set_scale(0.01);
    bn::sram::read(_loaded_sesh);
    _menu_options.emplace_back(-100, -30, "CONTINUE", _text_generator);



    if(_loaded_sesh.is_valid_object()){

        bn::string<64> session_summary;
        bn::ostringstream summary_stream(session_summary);
        summary_stream << "score: " << _loaded_sesh.get_score() << "  ";       
        summary_stream << "mans: " << _loaded_sesh.get_mans() << "  ";       
        summary_stream << "level: " << _loaded_sesh.get_level();       

        small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        small_generator.generate(-90, -15, session_summary, _menu_text_sprites);
        small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }else{
        small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        small_generator.generate(-90, -15, "no data saved.", _menu_text_sprites);
        small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }

    _menu_options.emplace_back(-100, 10, "NEW GAME", _text_generator);
    _menu_options.emplace_back(-100, 30, "HOW TO PLAY", _text_generator);
    
}

bn::optional<scene_type> menu_scene::update(){

    bn::optional<scene_type> result;

    if(_selection_anim_timer > 0){
        ++_selection_anim_timer;
        _flames_anim.update();
        _flames_scale.update();
        if(_selection_anim_timer > 30 ||  bn::keypad::a_pressed()) 
            result = scene_type::PLAY;
    }

    if(bn::keypad::a_pressed()){
            _selection_anim_timer = 1;
        if(_selected_option_index == 0){
            //load file
            if(_loaded_sesh.is_valid_object()){
                _sesh = _loaded_sesh;
                bn::sram::clear(sizeof(_sesh));
                BN_LOG("loaded the file");
                select();
            }else{
                _selection_anim_timer = 0;
                // probably provide some type of WRONG sound effect to provide feedback
            }
        }else if(_selected_option_index == 1){
            select();
        }else if(_selected_option_index == 2){
            //Tutorial level is lv. 0 
            _sesh.set_level(0);
            select();
        }


    }else if(bn::keypad::up_pressed()){
        --_selected_option_index;
    }else if(bn::keypad::down_pressed()){
        ++_selected_option_index;
    }

    if(bn::keypad::any_pressed()){
        _selected_option_index = _selected_option_index % _menu_options.size();
        _cursor.set_y(_menu_options.at(_selected_option_index).y());
    }

    return result;
}

void menu_scene::select(){
    _cursor.set_item(bn::sprite_items::trogdorhead, 1);
    _flames.set_visible(true);
    _flames.set_position(_cursor.position());
}

menu_option::menu_option(const bn::fixed &x, const bn::fixed &y, const char *text, bn::sprite_text_generator& _text_generator){
    _text_generator.set_left_alignment();

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
    _text_generator.generate(x - 1, y + 1, text, _text_sprites);    

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.generate(x, y, text, _text_sprites);    
}

}