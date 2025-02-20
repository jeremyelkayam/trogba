#include "dragon_select_scene.h"
#include "constants.h"
#include "bn_sprite_items_trogdor_variable_8x16_font.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_gray.h"
#include "bn_sprite_items_tutorial_arrow.h"
#include "sucks.h"
#include "trogdor.h"
#include "chiaroscuro.h"
#include "wormdingler.h"

#include <bn_keypad.h>
#include <bn_log.h>

#define SPACING 70
#define BACK_SPACING 45
#define MSATIME 10 //move selection animation time 
#define SELY 20
#define SELSCALE 2


namespace trog {

dragon_select_scene::dragon_select_scene(session_info &sesh, common_stuff &common_stuff) : 
        _index(0),
        _selection_timer(0),
        _selection_wait_time(120),
        _left_arrow(bn::sprite_items::tutorial_arrow.create_sprite(-50,20)),
        _right_arrow(bn::sprite_items::tutorial_arrow.create_sprite(50,20)),
        _title(false, 0, -65, "CHOOSE A DRAGON", bn::sprite_items::trogdor_variable_8x16_font_gray.palette_item()),
        _sesh(sesh),
        _common_stuff(common_stuff) {
    _selectable_dragons.emplace_back(
        dragon::TROGDOR, "TROGDOR", "none",
        bn::unique_ptr<player>(new trogdor(0,0,_sesh, false, _common_stuff, 0)),
        6, 6);
    _selectable_dragons.emplace_back(
        dragon::SUCKS, 
        "S IS FOR SUCKS", "ground pound",
        bn::unique_ptr<player>(new sucks(0,0,_sesh, false, _common_stuff, 0)),
        4, 8);
    _selectable_dragons.emplace_back(
        dragon::CHIAROSCURO, 
        "CHIAROSCURO", "none",
        bn::unique_ptr<player>(new chiaroscuro(0,0,_sesh, false, _common_stuff, 0)),
        10, 10);
    _selectable_dragons.emplace_back(
        dragon::WORMDINGLER, 
        "WORMDINGLER", "extendo tongue",
        bn::unique_ptr<player>(new wormdingler(0,0,_sesh, false, _common_stuff, 0)),
        10, 10);

    for(uint8_t i = 0; i < _selectable_dragons.size(); ++i){
        dragon_option &opt = _selectable_dragons.at(i);
        opt.player_entity->update_anim_action_when_not_moving(true);
        opt.player_entity->set_grayscale(1);

        if(opt.dragon_type == common_stuff.savefile.last_dragon_used){
            _index = i;
        }
    }
    for(uint8_t i = 0; i < _selectable_dragons.size(); ++i){
        dragon_option &opt = _selectable_dragons.at(i);
        opt.player_entity->set_x(dragon_xcor(i));
    }

    update_text();

    _selectable_dragons.at(_index).player_entity->set_grayscale(0);
    _selectable_dragons.at(_index).player_entity->set_scale(2);
    _selectable_dragons.at(_index).player_entity->set_y(SELY);
    _left_arrow.set_rotation_angle(90);
    _right_arrow.set_rotation_angle(270);  
    _left_arrow.put_above();  
    _right_arrow.put_above();
}

void dragon_select_scene::update_text(){
    _selected_text.clear();
    _common_stuff.text_generator.set_center_alignment();
    _common_stuff.text_generator.set_palette_item(WHITE_PALETTE);
    _common_stuff.text_generator.generate(0, -40, _selectable_dragons.at(_index).name, _selected_text);
}

bn::optional<scene_type> dragon_select_scene::update(){

    for(auto &opt : _selectable_dragons){
        opt.player_entity->update_anim();
    }
    bn::optional<scene_type> result;

    if(_selection_timer == 0){
        if(bn::keypad::left_pressed()){
            if(_index != 0) _index--;

            //looping code
            // if(_index == 0)
            //     _index = _selectable_dragons.size() - 1;
            // else _index--;
        }else if(bn::keypad::right_pressed()){
            if(_index != _selectable_dragons.size() - 1) _index++;

            //looping code
            // if(_index == _selectable_dragons.size() - 1)
            //     _index = 0;
            // else _index++;
        }

        if(bn::keypad::left_pressed() || bn::keypad::right_pressed()) {
            update_text();
            for(uint8_t z = 0; z < _selectable_dragons.size(); z++){
                bn::fixed ycor = (z == _index) ? SELY : 0;

                _selectable_dragons.at(z).player_entity->move_to(MSATIME, dragon_xcor(z), ycor);
            }    
            _selectable_dragons.at(_index).player_entity->grayscale_to(MSATIME, 0);
            _selectable_dragons.at(_index).player_entity->scale_to(MSATIME, 2);

            int8_t previous_selection_offset = bn::keypad::left_pressed() ? 1 : -1;

            _selectable_dragons.at(_index + previous_selection_offset).player_entity->grayscale_to(MSATIME, 1);
            _selectable_dragons.at(_index + previous_selection_offset).player_entity->scale_to(MSATIME, 1);
        }

        if(bn::keypad::a_pressed()){
            dragon dtype = _selectable_dragons.at(_index).dragon_type;
            _sesh.set_dragon(dtype);
            _common_stuff.savefile.last_dragon_used = dtype;
            bn::fixed vol_modifier = 1;
            if(_common_stuff.commentary.select_character(dtype)){
                vol_modifier = 0.2;
                _selection_wait_time = 150;
            }
            _selectable_dragons.at(_index).player_entity.get()->update_anim_action_when_not_moving(false);
            switch(dtype){
                case dragon::TROGDOR:
                    ((trogdor *) _selectable_dragons.at(_index).player_entity.get())->flex();
                    bn::sound_items::burninate.play(_common_stuff.savefile.options.sound_vol * vol_modifier);
                break;
                case dragon::SUCKS:
                    ((sucks *) _selectable_dragons.at(_index).player_entity.get())->stomp();
                    bn::sound_items::sucks_jingle.play(_common_stuff.savefile.options.sound_vol * vol_modifier);
                break;
                case dragon::WORMDINGLER:
                    // ((sucks *) _selectable_dragons.at(_index).player_entity.get())->stomp();
                    // bn::sound_items::sucks_jingle.play(_common_stuff.savefile.options.sound_vol * vol_modifier);
                break;
                default:
                break;
            }
            _selection_timer = 1;
        }
    }else if(_selection_timer == _selection_wait_time){
        result = scene_type::PLAY;
    }

    if(_selection_timer) ++_selection_timer;

    return result;
}

bn::fixed dragon_select_scene::dragon_xcor(uint8_t index){
    int8_t prs = index - _index;
    if(_index - 1 <= index && index <= _index + 1){
        return prs * SPACING;
    }else {
        return ((abs(prs) - 1) * BACK_SPACING + SPACING) * (abs(prs) / prs);
    }
}

}