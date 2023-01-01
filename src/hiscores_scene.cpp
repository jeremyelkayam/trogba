#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>
#include <bn_sram.h>
#include <bn_string.h>

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "hiscores_scene.h"
#include "constants.h"

namespace trog {

hiscores_scene::hiscores_scene(session_info &sesh, bn::sprite_text_generator& text_generator) : 
        _sesh(sesh),
        _text_generator(text_generator),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)) {


    init_high_scores_table();

    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(0, -72, "YE OLDE SCROLL OF HI-SCORES", _text_sprites);

    _text_generator.set_palette_item(BROWN_PALETTE);
    _text_generator.generate(-52, -55, "name", _text_sprites);
    _text_generator.generate(16, -55, "level", _text_sprites);
    _text_generator.generate(68, -55, "score", _text_sprites);

    for(int z = 0; z < _high_scores_table.size(); ++z){
    _text_generator.set_right_alignment();
        bn::string<3> place;
        bn::ostringstream place_string_stream(place);
        place_string_stream << z + 1 << ".";

        _text_generator.generate(-76, -43 + z*15, place, _text_sprites);   

        _text_generator.set_left_alignment();
        _text_generator.generate(-74, -43 + z*15, _high_scores_table.at(z).get_name(), _text_sprites);   

        _text_generator.set_center_alignment();
        bn::string<3> level;
        bn::ostringstream lv_string_stream(level);
        lv_string_stream << _high_scores_table.at(z).get_level();

        bn::string<12> score;
        bn::ostringstream score_string_stream(score);
        score_string_stream << _high_scores_table.at(z).get_score();

        _text_generator.generate(16, -43 + z*15, level, _text_sprites);   
        _text_generator.generate(68, -43 + z*15, score, _text_sprites);   
    }
    

}

bn::optional<scene_type> hiscores_scene::update(){
    bn::optional<scene_type> result;

    if(bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        result = scene_type::INSTRUCTIONS;
    }
    
    return result;
}

void hiscores_scene::init_high_scores_table(){
    _high_scores_table.emplace_back("THE CHEAT", 100, 6000);
    // _high_scores_table.emplace_back("POM POM", 90, 5000);
    // _high_scores_table.emplace_back("VIDLCTRX3", 80, 4000);
    // _high_scores_table.emplace_back("LTGTNJAXN", 75, 3000);
    // _high_scores_table.emplace_back("VIDLCTRX1", 60, 1987);
    // _high_scores_table.emplace_back("SHARPDENE", 50, 1000);
    // _high_scores_table.emplace_back("DJMANKWCZ", 40, 800);
    // _high_scores_table.emplace_back("COACH Z", 4, 10);
    
    // bn::sram::write_offset(_high_scores_table, sizeof(session_info));
}

high_score_entry::high_score_entry(bn::string<9> name, unsigned short level, unsigned short score) : 
    _level(level),
    _score(score) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append(name);    
}

bn::string<9> high_score_entry::get_name(){
    bn::string<9>result;
    for(char z : _name){
        result.push_back(z);
    }
    return result;
}

}