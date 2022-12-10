#include <bn_log.h>
#include <bn_color_effect.h>
#include <bn_sprite_palette_ptr.h>

#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_peasanthead.h"
#include "bn_sprite_items_peasanthead_grayscale.h"
#include "bn_regular_bg_items_burninometer.h"
#include "bn_regular_bg_items_burninometer_inverted.h"
#include "hud.h"


namespace trog {

hud::hud(session_info &sesh, bn::sprite_text_generator& generator, unsigned short trogmeter_max) : 
        _sesh(sesh),
        _text_generator(generator),
        _burninatemeter(bn::regular_bg_items::burninometer.create_bg(TROG_HUD_BURNINATEMETER_CENTER, -75)),
        _burninatemeter_invert(bn::regular_bg_items::burninometer_inverted.create_bg(TROG_HUD_BURNINATEMETER_CENTER, -75)),
        _burninatemeter_window(bn::rect_window::internal()),
        _enabled(true) {
    int trogmeter_start = TROG_HUD_TROGMETER_LEFTBOUND;
    for(int i = 0; i < trogmeter_max; i++){
        bn::sprite_ptr peasanthead_sprite = bn::sprite_items::peasanthead_grayscale.create_sprite(
            trogmeter_start + i*(TROG_HUD_PEASANT_HEAD_WIDTH + TROG_HUD_PEASANT_HEAD_SPACING), TROG_HUD_TROGMETER_YCOR);
        bn::sprite_palette_ptr peasanthead_palette = peasanthead_sprite.palette();
        _trogmeter_sprites.emplace_back(peasanthead_sprite);
    }

    //need to set up a window to make the trogmeter run out
    bn::window outside_window = bn::window::outside();
    
    outside_window.set_show_bg(_burninatemeter, false);

    _burninatemeter_window.set_boundaries(-80,TROG_HUD_BURNINATEMETER_LEFTBOUND,-72,TROG_HUD_TROGMETER_LEFTBOUND + TROG_HUD_BURNINATEMETER_WIDTH);

    _burninatemeter.set_visible(false);

    _burninatemeter_invert.set_visible(false);
    _burninatemeter_invert.put_below();

}


void hud::update_trogmeter(unsigned short trogmeter_value){
    for(int i = 0; i < TROG_TROGMETER_MAX; i++){
        // BN_LOG("trogmeter_value ", trogmeter_value);
        bn::sprite_ptr peasanthead_sprite = _trogmeter_sprites.at(i);
        if(i < trogmeter_value) {
            peasanthead_sprite.set_item(bn::sprite_items::peasanthead);
        }else{
            peasanthead_sprite.set_item(bn::sprite_items::peasanthead_grayscale);
        }
    }
}

void hud::set_sprite_arr_visible(bn::ivector<bn::sprite_ptr> &sprites, bool visible) {
    for(bn::sprite_ptr sprite : sprites) {
        sprite.set_visible(visible);
    }
}


void hud::update_burninatemeter(unsigned int burninate_time){
    if(burninate_time == 0) { 
        _burninatemeter.set_visible(false);
        _burninatemeter_invert.set_visible(false);
        set_sprite_arr_visible(_trogmeter_sprites, true);
    }else{
        BN_LOG("burninate meter visible");
        _burninatemeter.set_visible(true);
        _burninatemeter_invert.set_visible(true);
        bn::fixed time_percentage = burninate_time;
        time_percentage /= TROG_BURNINATE_TIME;
        _burninatemeter_window.set_right(TROG_HUD_BURNINATEMETER_LEFTBOUND + (TROG_HUD_BURNINATEMETER_WIDTH * time_percentage));
        set_sprite_arr_visible(_trogmeter_sprites, false);
    }

}

void hud::set_all_visible(bool visible){
    set_sprite_arr_visible(_score_text_sprites, visible);
    set_sprite_arr_visible(_mans_lv_text_sprites, visible);
    if(!_burninatemeter.visible()){
        set_sprite_arr_visible(_trogmeter_sprites, visible);        
    }
    _enabled = visible;
}

void hud::hide(){
    set_all_visible(false);
}

void hud::show(){
    set_all_visible(true);
}

void hud::update() {
    if(_enabled){

        _score_text_sprites.clear();
        _mans_lv_text_sprites.clear();

        _text_generator.set_palette_item(bn::sprite_items::trogdor_variable_8x16_font_red.palette_item());

        bn::string<64> score_str, mans_lv_str;
        bn::ostringstream score_string_stream(score_str);
        score_string_stream << _sesh.get_score();

        _text_generator.set_left_alignment();
        _text_generator.generate(-120, -76, score_str, _score_text_sprites);   

        //TODO: don't just right align this. The text should be right aligned and the numbers
        // should be left aligned
        bn::ostringstream mans_lv_string_stream(mans_lv_str);
        mans_lv_string_stream << "MANS:";
        mans_lv_string_stream << _sesh.get_mans();
        mans_lv_string_stream << " Lv:";
        mans_lv_string_stream << _sesh.get_level();

        _text_generator.set_right_alignment();
        _text_generator.generate(120, -76, mans_lv_str, _mans_lv_text_sprites);
        for(bn::sprite_ptr sprite : _mans_lv_text_sprites) {
            // need to do this for it to be covered up by the fire on the level win scene
            sprite.put_below();
        }
    }
}


}