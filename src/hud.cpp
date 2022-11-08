#include <bn_log.h>
#include <bn_color_effect.h>

#include "bn_regular_bg_items_titlegraphic.h"
#include "bn_sprite_items_peasanthead.h"
#include "hud.h"


namespace trog {

hud::hud(session_info &sesh, bn::sprite_text_generator& generator, unsigned short trogmeter_max) : 
        _sesh(sesh),
        _text_generator(generator){
    int trogmeter_start = -55;
    for(int i = 0; i < trogmeter_max; i++){
        _trogmeter.emplace_back(bn::sprite_items::peasanthead.create_sprite(trogmeter_start + i*9, -75));
    }
    BN_LOG("hi");
}


void hud::update_trogmeter(unsigned short trogmeter){

}

void hud::update_burninatemeter(unsigned int burninate_time){

}

void hud::update() {
    _score_text_sprites.clear();
    _mans_lv_text_sprites.clear();


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