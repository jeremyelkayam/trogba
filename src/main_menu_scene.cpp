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

main_menu_scene::main_menu_scene(session_info &sesh, common_stuff &common_stuff) : 
        menu_scene(common_stuff, bn::regular_bg_items::main_menu),
        _last_line2_option(2),
        _cursor(bn::sprite_items::trogdorhead.create_sprite(-100,-52)),
        _flames(bn::sprite_items::firebreath.create_sprite(-100,-30)),
        _troghammer_icon(bn::sprite_items::troghammer_icon.create_sprite(55,-50)),
        _trogmeter_degrade_icon(bn::sprite_items::trogmeter_degrade_icon.create_sprite(65,-50)),
        _flames_anim(bn::create_sprite_animate_action_forever(
                    _flames, 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3)),
        _flames_scale(_flames, SELECTION_ANIM_LENGTH, 4),
        _selection_anim_timer(0),
        _sesh(sesh),
        _scroll_speed(4) {
    bn::sound_items::trogador.play(_common_stuff.savefile.options.sound_vol);

    _troghammer_icon.set_visible(false);
    _trogmeter_degrade_icon.set_visible(false);

    _flames.set_visible(false);
    _flames.set_scale(0.01);
    common_stuff.text_generator.set_center_alignment();
    _menu_options.emplace_back(0, -52, "CONTINUE", 
        common_stuff.text_generator, scene_type::PLAY);

    common_stuff.small_generator.set_center_alignment();

    saved_session &loaded_sesh = common_stuff.savefile.session;

    if(loaded_sesh.exists){
        bn::string<64> session_summary;
        bn::ostringstream summary_stream(session_summary);
        summary_stream << "score: " << loaded_sesh.score << "  ";       
        summary_stream << "mans: " << loaded_sesh.mans << "  ";       
        summary_stream << "level: " << loaded_sesh.level;       

        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        common_stuff.small_generator.generate(0, -36, session_summary, _menu_text_sprites);
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());

        _troghammer_icon.set_visible(loaded_sesh.troghammer);
        _trogmeter_degrade_icon.set_visible(loaded_sesh.can_lose_trogmeter);
    }else{
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        common_stuff.small_generator.generate(0, -26, "no data saved.", _menu_text_sprites);
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }

    _menu_options.emplace_back(-72, 32, "NEW\nGAME", 
        common_stuff.text_generator, scene_type::DRAGON_SELECT);
    // _menu_options.emplace_back( "HOW TO PLAY", common_stuff.text_generator);
    _menu_options.emplace_back(1, 32, "OPTIONS", 
        common_stuff.text_generator, scene_type::OPTIONS);
    _menu_options.emplace_back(72, 32, "EXTRAS", 
        common_stuff.text_generator, scene_type::EXTRAS);
    // _menu_options.emplace_back(5, 77, "HI-SCORES", common_stuff.text_generator);
    // _menu_options.emplace_back(5, 107, "CREDITS", common_stuff.text_generator);
    // _menu_options.emplace_back(5, 137, "CUTSCENE VIEWER", common_stuff.text_generator);
}

bn::optional<scene_type> main_menu_scene::update(){

    bn::optional<scene_type> result = menu_scene::update();
    
    menu_option &current_option = _menu_options.at(_selected_option_index);

    if(_selection_anim_timer > 0){
        ++_selection_anim_timer;
        _flames_anim.update();
        _flames_scale.update();
        _flames_translate->update();
        if(_selection_anim_timer > SELECTION_ANIM_LENGTH ||  bn::keypad::a_pressed()){
            result = current_option.next_scene();
        } 
    }

    if(bn::keypad::a_pressed()){
            _selection_anim_timer = 1;
        if(_selected_option_index == 0){
            if(_common_stuff.savefile.session.exists){
                _sesh.import_save();
                select();
            }else{
                _selection_anim_timer = 0;
                bn::sound_items::deleted.play(_common_stuff.savefile.options.sound_vol);
            }
        }else if(_selected_option_index == 1){
            //Erase the existing session if you're starting a new game
            _common_stuff.savefile.session.exists = false;
            _sesh.reset();
        }

        if(_selected_option_index != 0){
                select();
        }


    }else if((bn::keypad::up_pressed() || bn::keypad::down_pressed()) && !_selection_anim_timer){
        if(_selected_option_index == 0){
            _selected_option_index = _last_line2_option;
        }else{
            _selected_option_index = 0;
        }
    }else if((bn::keypad::left_pressed()) && !_selection_anim_timer){
        if(_selected_option_index > 1){
            --_selected_option_index;
        }
    }else if((bn::keypad::right_pressed()) && !_selection_anim_timer){
        if(_selected_option_index != 0 && _selected_option_index < 3){
            ++_selected_option_index;
        }
    }

    if(bn::keypad::any_pressed() && _selected_option_index != 0)
    {
        _last_line2_option = _selected_option_index;
    }


    _cursor.set_y(current_option.y());
    _cursor.set_x(current_option.x() - (current_option.width()/2 + 5));

    //old scrolling code
    if(_cursor.y() < -62 || _cursor.y() > 50){

        bn::fixed offset = _cursor.y() < -62 ? _scroll_speed : -_scroll_speed;
        for(menu_option &opt : _menu_options){
            opt.set_y(opt.y() + offset);
        }
        for(bn::sprite_ptr &sprite : _menu_text_sprites){
            sprite.set_y(sprite.y() + offset);
        }
        _cursor.set_y(_cursor.y() + offset);
        _flames.set_y(_flames.y() + offset);
        _troghammer_icon.set_y(_troghammer_icon.y() + offset);
        _trogmeter_degrade_icon.set_y(_trogmeter_degrade_icon.y() + offset);
        _bg.set_y(_bg.y() + offset);
    }

    // highlighting selected menu option
    for(uint8_t i = 0; i < _menu_options.size(); i++){
        if(i == _selected_option_index) {
            _menu_options.at(i).turn_red();
        } else {
            _menu_options.at(i).turn_white();
        }
    }




    return result;
}

void main_menu_scene::select(){
    _cursor.set_item(bn::sprite_items::trogdorhead, 1);
    _flames.set_visible(true);
    _flames.put_above();
    _flames.set_position(_cursor.position().x() + 2, _cursor.position().y() + 2);
    _flames_translate = bn::sprite_move_to_action(_flames, SELECTION_ANIM_LENGTH, _flames.position().x() + 40, _flames.position().y());
    bn::sound_items::burningcottage.play(_common_stuff.savefile.options.sound_vol);
}


}