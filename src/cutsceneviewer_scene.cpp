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

cutsceneviewer_scene::cutsceneviewer_scene(
    session_info &sesh, common_stuff &common_stuff, 
    const bn::fixed &top_ycor) : 
        _sesh(sesh),
        _common_stuff(common_stuff),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)),
        _black_generator(small_font_black),
        _norm_palette(bn::sprite_palette_ptr::create(
            bn::sprite_items::serif_font_black.palette_item())),
        _selected_palette(bn::sprite_palette_ptr::create_new(
            bn::sprite_items::serif_font_black.palette_item())),
        _trans_atts(
            []{
                bn::array<bn::blending_transparency_attributes, bn::display::height()> transparency_attributes;
                for(int line = 0; line < bn::display::height(); ++line)
                {
                    
                    bn::fixed trans_alpha;
                    if(line < OPAQUE_START - TRANSITION_LINES)
                    {
                        trans_alpha = 0;
                    }
                    else if(line < OPAQUE_START)
                    {
                        trans_alpha = 1 - bn::fixed(OPAQUE_START - line) / TRANSITION_LINES;
                    }
                    else if(line < OPAQUE_END)
                    {
                        trans_alpha = 1;
                    }
                    else if(line < OPAQUE_END + TRANSITION_LINES)
                    {
                        trans_alpha = 1 - bn::fixed(line - OPAQUE_END) / TRANSITION_LINES;
                    }
                    else
                    {
                        trans_alpha = 0;
                    }
                    transparency_attributes[line].set_transparency_alpha(trans_alpha);
                }

                return transparency_attributes;
            }()
        ),
        _bta_hbe(
            bn::blending_transparency_attributes_hbe_ptr::create(
                _trans_atts
            )
        ),
        _index(0),
        _top_ycor(top_ycor)
{
    _black_generator.set_z_order(1);
    _selected_palette.set_rotate_count(2);
    
    bn::sprite_text_generator serif_gen(serif_font_red); 
    serif_gen.set_center_alignment();
    serif_gen.generate(0, -72, "YE OLDE CUTSCENE VIEWER", _header_sprites);

    for(bn::sprite_ptr &s : _header_sprites)
    {
        s.set_palette(_norm_palette);
    }


    for(int z = 0; z < _common_stuff.cutscene_levels.size(); z++){
        if(_common_stuff.savefile.unlocked_cutscenes[z]){
            int lv = _common_stuff.cutscene_levels.at(z).first;
            bn::string<32> nm = _common_stuff.cutscene_levels.at(z).second;
            _options_vec.emplace_back(bn::vector<bn::sprite_ptr, 2>(), bn::vector<bn::sprite_ptr, 6>(), nm, lv);
        }
    }
    _options_vec.emplace_back(bn::vector<bn::sprite_ptr, 2>(), bn::vector<bn::sprite_ptr, 6>(), bn::string<32>("back"), 0);

    for(int z = 0; z < _options_vec.size(); ++z)
    {        
        create_line(_black_generator, z);
        for(bn::sprite_ptr &s : _options_vec.at(z).lv_text_sprites)
        {
            s.set_blending_enabled(true);
        }
        for(bn::sprite_ptr &s : _options_vec.at(z).title_text_sprites)
        {
            s.set_blending_enabled(true);
        }
    }


    _black_generator.set_left_alignment();

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
        bn::sprite_palette_ptr &pal = (z == _index ? _selected_palette : 
            _norm_palette);
        for(bn::sprite_ptr &s : _options_vec.at(z).lv_text_sprites)
        {
            s.set_palette(pal);
        }

        for(bn::sprite_ptr &s : _options_vec.at(z).title_text_sprites)
        {
            s.set_palette(pal);
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

    if(_options_vec.at(_index).lv_text_sprites.at(0).y() > 
        OPAQUE_END - 80)
    {
        move_all(-4);
    }
    else if(_options_vec.at(_index).lv_text_sprites.at(0).y() <
        OPAQUE_START - 74)
    {
        move_all(4);
    }
        
    return result;
}

void cutsceneviewer_scene::create_line(bn::sprite_text_generator &gen, int index)
{
    bn::fixed yoffset = (index) * SPACING;
    if(index == _options_vec.size() - 1){
        gen.generate(-90, _top_ycor + yoffset, _options_vec.at(index).title, _options_vec.at(index).lv_text_sprites);
    }else{
        bn::string<32> opt_str;
        bn::ostringstream opt_string_stream(opt_str);

        opt_string_stream << "Lv" << _options_vec.at(index).level << ".";
        

        gen.generate(-90, _top_ycor + yoffset, opt_str, _options_vec.at(index).lv_text_sprites);
        gen.generate(-55, _top_ycor + yoffset, _options_vec.at(index).title, _options_vec.at(index).title_text_sprites);
    }
}

void cutsceneviewer_scene::move_all(const bn::fixed &move_by)
{
    for(cvoption &opt : _options_vec)
    {
        for(bn::sprite_ptr &s : opt.lv_text_sprites)
        {
            s.set_y(s.y() + move_by);
        }
        for(bn::sprite_ptr &s : opt.title_text_sprites)
        {
            s.set_y(s.y() + move_by);
        }
    }
}


}