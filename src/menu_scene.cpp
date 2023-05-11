#include <bn_sram.h>
#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sound_items.h>

#include "menu_scene.h"
#include "bn_sprite_items_trogdorhead.h"
#include "bn_sprite_items_firebreath.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_regular_bg_items_main_menu.h"

#define SELECTION_ANIM_LENGTH 30

namespace trog {

menu_scene::menu_scene(session_info &sesh, common_stuff &common_stuff) : 
        _text_generator(common_stuff.text_generator),
        // _small_text_generator(variable_8x8_sprite_font),
        _cursor(bn::sprite_items::trogdorhead.create_sprite(-100,-52)),
        _flames(bn::sprite_items::firebreath.create_sprite(-100,-30)),
        _flames_anim(bn::create_sprite_animate_action_forever(
                    _flames, 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3)),
        _flames_scale(_flames, SELECTION_ANIM_LENGTH, 4),
        _bg(bn::regular_bg_items::main_menu.create_bg(0, 45)),
        _selected_option_index(0),
        _selection_anim_timer(0),
        _sesh(sesh) {
    bn::sound_items::trogador.play(TROG_DEFAULT_VOLUME);


    _flames.set_visible(false);
    _flames.set_scale(0.01);
    bn::sram::read(_loaded_sesh);
    _text_generator.set_center_alignment();
    _menu_options.emplace_back(5, -62, "CONTINUE", _text_generator);

    common_stuff.small_generator.set_center_alignment();

    if(_loaded_sesh.is_valid_object()){
        bn::string<64> session_summary;
        bn::ostringstream summary_stream(session_summary);
        summary_stream << "score: " << _loaded_sesh.get_score() << "  ";       
        summary_stream << "mans: " << _loaded_sesh.get_mans() << "  ";       
        summary_stream << "level: " << _loaded_sesh.get_level();       

        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        common_stuff.small_generator.generate(5, -46, session_summary, _menu_text_sprites);
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }else{
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        common_stuff.small_generator.generate(5, -46, "no data saved.", _menu_text_sprites);
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }

    _menu_options.emplace_back(5, -15, "NEW GAME", _text_generator);
    _menu_options.emplace_back(5, 17, "HOW TO PLAY", _text_generator);
    _menu_options.emplace_back(5, 47, "OPTIONS", _text_generator);
    
}

bn::optional<scene_type> menu_scene::update(){

    bn::optional<scene_type> result;

    if(_selection_anim_timer > 0){
        ++_selection_anim_timer;
        _flames_anim.update();
        _flames_scale.update();
        _flames_translate->update();
        if(_selection_anim_timer > SELECTION_ANIM_LENGTH ||  bn::keypad::a_pressed()) 
            result = scene_type::PLAY;
    }

    if(bn::keypad::a_pressed()){
            _selection_anim_timer = 1;
        if(_selected_option_index == 0){
            //load file
            if(_loaded_sesh.is_valid_object()){
                _sesh = _loaded_sesh;
                BN_LOG("loaded the file");
                select();
            }else{
                _selection_anim_timer = 0;
                // probably provide some type of WRONG sound effect to provide feedback
                bn::sound_items::deleted.play(TROG_DEFAULT_VOLUME);
            }
        }else if(_selected_option_index == 1){
            select();
        }else if(_selected_option_index == 2){
            //Tutorial level is lv. 0 
            _sesh.set_level(0);
            select();
        }


    }else if(bn::keypad::up_pressed() && !_selection_anim_timer){
        if(_selected_option_index == 0){
            _selected_option_index = _menu_options.size() - 1;
        }else{
            --_selected_option_index;
        }
    }else if(bn::keypad::down_pressed() && !_selection_anim_timer){
        if(_selected_option_index == _menu_options.size() - 1){
            _selected_option_index = 0;
        }else{
            ++_selected_option_index;
        }
    }
    _cursor.set_y(_menu_options.at(_selected_option_index).y());

    //highlighting selected menu option
    // for(uint8_t i = 0; i < _menu_options.size(); i++){
    //     if(i == _selected_option_index) {
    //         _menu_options.at(i).turn_red();
    //     } else {
    //         _menu_options.at(i).turn_white();
    //     }
    // }




    return result;
}

void menu_scene::select(){
    _cursor.set_item(bn::sprite_items::trogdorhead, 1);
    _flames.set_visible(true);
    _flames.put_above();
    _flames.set_position(_cursor.position().x() + 2, _cursor.position().y() + 2);
    _flames_translate = bn::sprite_move_to_action(_flames, SELECTION_ANIM_LENGTH, _flames.position().x() + 40, _flames.position().y());
    bn::sound_items::burningcottage.play(TROG_DEFAULT_VOLUME);
}

menu_option::menu_option(const bn::fixed &x, const bn::fixed &y, const char *text, bn::sprite_text_generator& _text_generator){
    _text_generator.set_center_alignment();

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
    _text_generator.generate(x - 1, y + 1, text, _text_sprites);    

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    _text_generator.generate(x, y, text, _text_sprites);    

    _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    _text_generator.generate(x, y, text, _red_text_sprites);   
    turn_white();
}

void menu_option::turn_red(){
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_visible(true);
}

void menu_option::turn_white(){
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_visible(false);
}

}