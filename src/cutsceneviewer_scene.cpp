#include <bn_keypad.h>
#include <bn_string.h>
#include <bn_log.h>

#include "cutsceneviewer_scene.h"
#include "constants.h"

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "bn_sprite_items_checkbox.h"
#include "bn_sprite_items_slider_bar.h"
#include "bn_sprite_items_volume_graph.h"
#include "small_fonts.h"
#include "serif_fonts.h"

namespace trog {

cutsceneviewer_scene::cutsceneviewer_scene(session_info &sesh, common_stuff &common_stuff) : 
        _sesh(sesh),
        _common_stuff(common_stuff),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)),
        _black_generator(small_font_black),
        _red_generator(small_font_red),
        _top_line(0),
        _index(0)
{
    _black_generator.set_z_order(1);
    _red_generator.set_z_order(1);
    
    bn::sprite_text_generator serif_gen(serif_font_red); 
    serif_gen.set_center_alignment();
    serif_gen.generate(0, -72, "YE OLDE CUTSCENE VIEWER", _header_sprites);


    for(int z = 0; z < _common_stuff.cutscene_levels.size(); z++){
        if(_common_stuff.savefile.unlocked_cutscenes[z]){
            int lv = _common_stuff.cutscene_levels.at(z).first;
            bn::string<32> nm = _common_stuff.cutscene_levels.at(z).second;
            _options_vec.emplace_back(bn::vector<bn::sprite_ptr, 2>(), bn::vector<bn::sprite_ptr, 6>(), nm, lv);
        }
    }
    _options_vec.emplace_back(bn::vector<bn::sprite_ptr, 2>(), bn::vector<bn::sprite_ptr, 6>(), bn::string<32>("back"), 0);


    _black_generator.set_left_alignment();
    _red_generator.set_left_alignment();

    for(int z = 0; z < _options_vec.size(); z++){
        //trick to set our index to the last selected option when returning from this menu
        if(_sesh.get_level() == _options_vec.at(z).level){
            _index = z;
        }
    }
    update_selection();

}

void cutsceneviewer_scene::update_selection(){
    for(int z = 0; z < _options_vec.size(); z++){
        _options_vec.at(z).lv_text_sprites.clear();
        _options_vec.at(z).title_text_sprites.clear();

        if(z >= _top_line && z <= _top_line + MAX_LINES_VISIBLE)
        {
            if(z == _index) create_line(_red_generator, z);
            else create_line(_black_generator, z);
            BN_LOG("drew line ", z);
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

        if(_index < _top_line)
        {
            _top_line = _index;
        }
        else if(_index >= _top_line + MAX_LINES_VISIBLE)
        {
            _top_line = _index - MAX_LINES_VISIBLE;
        }
        update_selection();
    }

    if(bn::keypad::a_pressed()){
        if(_index == _options_vec.size() - 1){
            result = scene_type::EXTRAS;
        }else{
            result = scene_type::MOVIE;
            _sesh.set_level(_options_vec.at(_index).level);
        }
    }

    if(bn::keypad::b_pressed())
    {
        result = scene_type::EXTRAS;
    }
        
    return result;
}

void cutsceneviewer_scene::create_line(bn::sprite_text_generator &gen, int index)
{
    bn::fixed yoffset = (index - _top_line) * 9;
    if(index == _options_vec.size() - 1){
        gen.generate(-90, -52 + yoffset, _options_vec.at(index).title, _options_vec.at(index).lv_text_sprites);
    }else{
        bn::string<32> opt_str;
        bn::ostringstream opt_string_stream(opt_str);

        opt_string_stream << "Lv" << _options_vec.at(index).level << ".";
        

        gen.generate(-90, -52 + yoffset, opt_str, _options_vec.at(index).lv_text_sprites);
        gen.generate(-55, -52 + yoffset, _options_vec.at(index).title, _options_vec.at(index).title_text_sprites);
    }
}


}