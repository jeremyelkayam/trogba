#include "bn_regular_bg_items_titlegraphic.h"
#include "hud.h"

namespace trog {

hud::hud(session_info &sesh, bn::sprite_text_generator& generator) : 
        _sesh(sesh),
        _text_generator(generator){
    frames = 0;
}

void hud::update() {
    _score_text_sprites.clear();
    _mans_lv_text_sprites.clear();


    if(frames == 60) { 
        _sesh.level++;
        frames = 0;
    }
    frames++;

    bn::string<64> score_str, mans_lv_str;
    bn::ostringstream score_string_stream(score_str);
    score_string_stream << " SCORE: ";
    score_string_stream << _sesh.score;

    _text_generator.set_left_alignment();
    _text_generator.generate(-125, -75, score_str, _score_text_sprites);   


    bn::ostringstream mans_lv_string_stream(mans_lv_str);
    mans_lv_string_stream << "MANS: ";
    mans_lv_string_stream << _sesh.mans;
    mans_lv_string_stream << " LV: ";
    mans_lv_string_stream << _sesh.level;

    _text_generator.set_right_alignment();
    _text_generator.generate(120, -75, mans_lv_str, _mans_lv_text_sprites);   

}

}