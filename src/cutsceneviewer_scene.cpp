#include <bn_keypad.h>
#include <bn_string.h>
#include <bn_log.h>

#include "cutsceneviewer_scene.h"
#include "constants.h"

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_brown.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x8_font.h"
#include "bn_sprite_items_checkbox.h"
#include "bn_sprite_items_slider_bar.h"
#include "bn_sprite_items_volume_graph.h"

namespace trog {

cutsceneviewer_scene::cutsceneviewer_scene(session_info &sesh, common_stuff &common_stuff) : 
        _sesh(sesh),
        _common_stuff(common_stuff),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)),
        _index(0) {
    _common_stuff.text_generator.set_center_alignment();
    _common_stuff.text_generator.set_palette_item(RED_PALETTE);
    _common_stuff.text_generator.generate(0, -72, "YE OLDE CUTSCENE VIEWER", _header_sprites);


    for(bn::pair<uint8_t, bn::string<32>> cutscene : _common_stuff.cutscene_levels){
        _options_vec.emplace_back(bn::vector<bn::sprite_ptr, 8>(), cutscene.second, cutscene.first);
    }

    bn::sprite_text_generator &txtgen = common_stuff.small_generator;

    for(uint8_t z = 0; z < _options_vec.size(); z++){
        txtgen.set_left_alignment();
        txtgen.set_palette_item(BLACK_PALETTE);

        bn::string<32> opt_str;
        bn::ostringstream opt_string_stream(opt_str);

        opt_string_stream << "Lv" << _options_vec.at(z).level << ": " << _options_vec.at(z).title;

        txtgen.generate(-70, -50 + 9 * z, opt_str, _options_vec.at(z).text_sprites);
    }
    update_selection();

}

void cutsceneviewer_scene::update_selection(){
    for(uint8_t z = 0; z < _options_vec.size(); z++){
        for(bn::sprite_ptr &sprite : _options_vec.at(z).text_sprites){
            if(z == _index) sprite.set_palette(RED_PALETTE);
            else sprite.set_palette(BLACK_PALETTE);
        }
    }
}

bn::optional<scene_type> cutsceneviewer_scene::update(){
    bn::optional<scene_type> result;

    //this logic pattern is kinda lame but idk what to do bout it 
    if(bn::keypad::up_pressed() || bn::keypad::down_pressed()){

        if(bn::keypad::up_pressed()){
            if(_index == 0) _index = _options_vec.size();
            _index--;
        }else if(bn::keypad::down_pressed()){
            _index++;
            if(_index == _options_vec.size()) _index = 0;
        }
        update_selection();

    }

    if(bn::keypad::a_pressed()){
        result = scene_type::MOVIE;
        _sesh.set_level(_options_vec.at(_index).level);

    }
        
    return result;
}


}