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
        _selectable_letters({'A','B','C','D','E','F','G','H','I','J','K','L','M','N',
                             'O','P','Q','R','S','T','U','V','W','X','Y','Z',' ','!','?'}),
        _sesh(sesh),
        _text_generator(text_generator),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64)), 
        _high_scores_table(
            {high_score_entry("THE CHEAT", 100, 6000),
            high_score_entry("POM POM", 90, 5000),
            high_score_entry("VIDLCTRX3", 80, 4000),
            high_score_entry("LTGTNJAXN", 75, 3000),
            high_score_entry("VIDLCTRX1", 60, 1987),
            high_score_entry("SHARPDENE", 50, 1000),
            high_score_entry("DJMANKWCZ", 40, 800),
            high_score_entry("COACH Z", 4, 10)}),
        _score_entry_index_in_table(-1) {
    
    //Initialize our format tag
    bn::istring_base expected_format_tag_istring(_format_tag._data);
    bn::ostringstream expected_format_tag_stream(expected_format_tag_istring);
    expected_format_tag_stream.append("TROG_HISCORES");

    //Load the table if applicable
    load_high_scores_table();
    

    //Propagate the player's score within the scores list (if applicable)
    if(_sesh.get_score() > _high_scores_table[7].get_score()){
        BN_LOG("you got a high score: ", _sesh.get_score());
        _high_scores_table[7] = high_score_entry("", _sesh.get_level(), _sesh.get_score());
        _score_entry_index_in_table=7;
        for(int z = 6; z >= 0; --z){
            high_score_entry& current = _high_scores_table[z];
            high_score_entry& previous = _high_scores_table[z+1];
            if(previous.get_score() > current.get_score()){
                bn::swap(previous, current);
                _score_entry_index_in_table=z;
            }
        }
    }
    BN_LOG("score entry at index ", _score_entry_index_in_table);

    draw_high_scores_table();

}

void hiscores_scene::draw_high_scores_table(){

    bn::string<64> title_text;
    if(_score_entry_index_in_table > -1){
        title_text = "ENTER YOUR NAME!";
    }else{
        title_text = "YE OLDE SCROLL OF HI-SCORES";
    }

    _text_generator.set_center_alignment();
    _text_generator.set_palette_item(RED_PALETTE);
    _text_generator.generate(0, -72, title_text, _text_sprites);

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

    if(_score_entry_index_in_table != -1)

    if(bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        result = scene_type::INSTRUCTIONS;
    }
    
    return result;
}

void hiscores_scene::load_high_scores_table(){

    bn::array<char, 16> loaded_format_tag;
    bn::sram::read_offset(loaded_format_tag, sizeof(_sesh));

    if(_format_tag == loaded_format_tag){
        BN_LOG("memory is formatted. loading now");
        bn::sram::read_offset(_high_scores_table, sizeof(_sesh) + sizeof(_format_tag));
    }
}

high_score_entry::high_score_entry(bn::string<9> name, unsigned short level, unsigned short score) : 
    _level(level),
    _score(score) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append(name);    
}

high_score_entry::high_score_entry() : 
    _level(0),
    _score(0) {

    bn::istring_base name_istring(_name._data);
    bn::ostringstream name_stream(name_istring);
    name_stream.append("DUMMY");    
}

bn::string<9> high_score_entry::get_name(){
    bn::string<9>result;
    //go until you start hitting null chars
    for(int z=0; _name[z] != 0; ++z){
        result.append(_name[z]);
    }

    return result;
}

}