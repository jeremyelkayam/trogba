#include <bn_log.h>
#include <bn_color_effect.h>
#include <bn_sprite_palette_ptr.h>

#include "bn_sprite_items_peasanthead.h"
#include "bn_sprite_items_peasanthead_grayscale.h"
#include "bn_regular_bg_items_burninometer.h"
#include "hud.h"


namespace trog {

hud::hud(session_info &sesh, bn::sprite_text_generator& generator, unsigned short trogmeter_max) : 
        _sesh(sesh),
        _text_generator(generator),
        _burninatemeter(bn::regular_bg_items::burninometer.create_bg(-10, -75)){
    int trogmeter_start = TROG_HUD_TROGMETER_LEFTBOUND;
    for(int i = 0; i < trogmeter_max; i++){
        bn::sprite_ptr peasanthead_sprite = bn::sprite_items::peasanthead_grayscale.create_sprite(
            trogmeter_start + i*(TROG_HUD_PEASANT_HEAD_WIDTH + TROG_HUD_PEASANT_HEAD_SPACING), TROG_HUD_TROGMETER_YCOR);
        bn::sprite_palette_ptr peasanthead_palette = peasanthead_sprite.palette();
        _trogmeter.emplace_back(peasanthead_sprite);
    }
    _burninatemeter.set_visible(false);
    BN_LOG("hi");
}


void hud::update_trogmeter(unsigned short trogmeter_value){
    for(int i = 0; i < TROG_TROGMETER_MAX; i++){
        // BN_LOG("trogmeter_value ", trogmeter_value);
        bn::sprite_ptr peasanthead_sprite = _trogmeter.at(i);
        if(i < trogmeter_value) {
            peasanthead_sprite.set_item(bn::sprite_items::peasanthead);
        }else{
            peasanthead_sprite.set_item(bn::sprite_items::peasanthead_grayscale);
        }
    }
}
void hud::hide_trogmeter(){
    for(int i = 0; i < TROG_TROGMETER_MAX; i++){
        bn::sprite_ptr peasanthead_sprite = _trogmeter.at(i);
        peasanthead_sprite.set_visible(false);
    }    
}
void hud::show_trogmeter(){
    for(int i = 0; i < TROG_TROGMETER_MAX; i++){
        bn::sprite_ptr peasanthead_sprite = _trogmeter.at(i);
        peasanthead_sprite.set_visible(true);
    }    
}

void hud::update_burninatemeter(unsigned int burninate_time){
    if(burninate_time == 0) { 
        _burninatemeter.set_visible(false);
        show_trogmeter();
    }else{
        _burninatemeter.set_visible(true);
        hide_trogmeter();
    }

}

void hud::update() {
    _score_text_sprites.clear();
    _mans_lv_text_sprites.clear();


    bn::string<64> score_str, mans_lv_str;
    bn::ostringstream score_string_stream(score_str);
    score_string_stream << "PTS:";
    score_string_stream << _sesh.score;

    _text_generator.set_left_alignment();
    _text_generator.generate(-120, -75, score_str, _score_text_sprites);   


    bn::ostringstream mans_lv_string_stream(mans_lv_str);
    mans_lv_string_stream << "MANS:";
    mans_lv_string_stream << _sesh.mans;
    mans_lv_string_stream << " LV:";
    mans_lv_string_stream << _sesh.level;

    _text_generator.set_right_alignment();
    _text_generator.generate(120, -75, mans_lv_str, _mans_lv_text_sprites);   

}

}