#include <bn_string.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sound_items.h>

#include "menu_scene.h"
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

menu_scene::menu_scene(session_info &sesh, common_stuff &common_stuff) : 
        _common_stuff(common_stuff),
        // _small_text_generator(variable_8x8_sprite_font),
        _cursor(bn::sprite_items::trogdorhead.create_sprite(-100,-52)),
        _flames(bn::sprite_items::firebreath.create_sprite(-100,-30)),
        _troghammer_icon(bn::sprite_items::troghammer_icon.create_sprite(45,-60)),
        _trogmeter_degrade_icon(bn::sprite_items::trogmeter_degrade_icon.create_sprite(55,-60)),
        _flames_anim(bn::create_sprite_animate_action_forever(
                    _flames, 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3)),
        _flames_scale(_flames, SELECTION_ANIM_LENGTH, 4),
        _bg(bn::regular_bg_items::main_menu.create_bg(0, 45)),
        _selected_option_index(0),
        _selection_anim_timer(0),
        _sesh(sesh),
        _scroll_speed(4) {
    bn::sound_items::trogador.play(_common_stuff.savefile.options.sound_vol);

    _troghammer_icon.set_visible(false);
    _trogmeter_degrade_icon.set_visible(false);

    _flames.set_visible(false);
    _flames.set_scale(0.01);
    common_stuff.text_generator.set_center_alignment();
    _menu_options.emplace_back(5, -62, "CONTINUE", common_stuff.text_generator);

    common_stuff.small_generator.set_center_alignment();

    saved_session &loaded_sesh = common_stuff.savefile.session;

    if(loaded_sesh.exists){
        BN_LOG("SESSION EXISTS! Loading in the datum");
        bn::string<64> session_summary;
        bn::ostringstream summary_stream(session_summary);
        summary_stream << "score: " << loaded_sesh.score << "  ";       
        summary_stream << "mans: " << loaded_sesh.mans << "  ";       
        summary_stream << "level: " << loaded_sesh.level;       

        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        common_stuff.small_generator.generate(5, -46, session_summary, _menu_text_sprites);
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());

        _troghammer_icon.set_visible(loaded_sesh.troghammer);
        _trogmeter_degrade_icon.set_visible(loaded_sesh.can_lose_trogmeter);
    }else{
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
        common_stuff.small_generator.generate(5, -46, "no data saved.", _menu_text_sprites);
        common_stuff.small_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    }

    _menu_options.emplace_back(5, -15, "NEW GAME", common_stuff.text_generator);
    _menu_options.emplace_back(5, 17, "HOW TO PLAY", common_stuff.text_generator);
    _menu_options.emplace_back(5, 47, "OPTIONS", common_stuff.text_generator);
    _menu_options.emplace_back(5, 77, "HI-SCORES", common_stuff.text_generator);
    _menu_options.emplace_back(5, 107, "CREDITS", common_stuff.text_generator);
}

bn::optional<scene_type> menu_scene::update(){

    bn::optional<scene_type> result;

    if(_selection_anim_timer > 0){
        ++_selection_anim_timer;
        _flames_anim.update();
        _flames_scale.update();
        _flames_translate->update();
        if(_selection_anim_timer > SELECTION_ANIM_LENGTH ||  bn::keypad::a_pressed()){
            if(_selected_option_index == 3){
                result = scene_type::OPTIONS;
            }else if(_selected_option_index == 4){
                result = scene_type::HISCORES;
            }else if(_selected_option_index == 5){
                result = scene_type::CREDITS;
            }else{
                result = scene_type::PLAY;
            }
        } 
        if(bn::keypad::select_pressed()) _sesh.set_dragon(dragon::SUCKS);
    }

    if(bn::keypad::a_pressed()){
            _selection_anim_timer = 1;
        if(_selected_option_index == 0){
            if(_common_stuff.savefile.session.exists){
                _sesh.import_save();
                BN_LOG("loaded the file");
                select();
            }else{
                _selection_anim_timer = 0;
                bn::sound_items::deleted.play(_common_stuff.savefile.options.sound_vol);
            }
        }else if(_selected_option_index == 1 || _selected_option_index == 2){
            //Erase the existing session if you're starting a new game
            _common_stuff.savefile.session.exists = false;
            _sesh.reset();
        }
        
        if(_selected_option_index == 2){
            _sesh.set_level(0);
        }

        if(_selected_option_index != 0){
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

void menu_scene::select(){
    _cursor.set_item(bn::sprite_items::trogdorhead, 1);
    _flames.set_visible(true);
    _flames.put_above();
    _flames.set_position(_cursor.position().x() + 2, _cursor.position().y() + 2);
    _flames_translate = bn::sprite_move_to_action(_flames, SELECTION_ANIM_LENGTH, _flames.position().x() + 40, _flames.position().y());
    bn::sound_items::burningcottage.play(_common_stuff.savefile.options.sound_vol);
}

menu_option::menu_option(const bn::fixed &x, const bn::fixed &y, const char *text, bn::sprite_text_generator& text_generator){
    text_generator.set_center_alignment();

    text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item());
    text_generator.generate(x - 1, y + 1, text, _drop_shadow_sprites);    

    text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font.palette_item());
    text_generator.generate(x, y, text, _text_sprites);    

    text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());
    text_generator.generate(x, y, text, _red_text_sprites);   
    turn_white();
}

void menu_option::turn_red(){
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_visible(true);
}

void menu_option::turn_white(){
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_visible(false);
}

void menu_option::set_y(const bn::fixed &y){
    for(bn::sprite_ptr &sprite : _text_sprites) sprite.set_y(y);
    for(bn::sprite_ptr &sprite : _drop_shadow_sprites) sprite.set_y(y + 1);
    for(bn::sprite_ptr &sprite : _red_text_sprites) sprite.set_y(y);
}

}