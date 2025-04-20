#include <bn_music_items.h>
#include <bn_log.h>
#include <bn_sprite_ptr.h>
#include <bn_keypad.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include <bn_sram.h>

#include "char_unlock_scene.h"
#include "sb_commentary.h"

#include "bn_regular_bg_items_trogsmile.h"
#include "bn_regular_bg_items_sucksmile.h"
#include "bn_regular_bg_items_chiarosmile.h"
#include "bn_regular_bg_items_wormsmile.h"
#include "bn_sprite_items_nose_smoke.h"
#include "bn_sprite_items_cottagefire.h"
#include "bn_sprite_items_a_button_prompt.h"
#include "serif_fonts.h"
#include "level_data.h"


namespace trog {

char_unlock_scene::char_unlock_scene(session_info &sesh, 
    common_stuff &common_stuff, const scene_type &next_scene) : 
        _happy_dragon(bn::regular_bg_items::trogsmile.create_bg(8, 61)),
        _a_button(bn::sprite_items::a_button_prompt.create_sprite(113,71)),
        _a_button_anim(bn::create_sprite_animate_action_forever(
            _a_button, 30, bn::sprite_items::a_button_prompt.tiles_item(), 0, 1)),
        _sesh(sesh),
        _common_stuff(common_stuff),
        _next_scene(next_scene) {

    _a_button.set_visible(false);

    bn::sprite_text_generator serif_white(serif_font_white);

    serif_white.set_center_alignment();

    // common_stuff.commentary.level_win_scene(_sesh.get_dragon());


    if(sesh.get_level() != 0 && sesh.get_level() != 100){
        save();
    }
    bn::music_item jingle = bn::music_items::trog;
    switch(_sesh.get_dragon()){
        case dragon::TROGDOR:
            _happy_dragon.set_item(bn::regular_bg_items::trogsmile);
            
        break;
        case dragon::SUCKS:
            _happy_dragon.set_item(bn::regular_bg_items::sucksmile);

            // jingle = bn::sound_items::sucks_jingle;
        break;
        case dragon::CHIAROSCURO:
            _happy_dragon.set_item(bn::regular_bg_items::chiarosmile);
        break;
        case dragon::WORMDINGLER:
            _happy_dragon.set_item(bn::regular_bg_items::wormsmile);
        break;
        default:
            BN_ERROR("Invalid dragon type found in session info");
        break;
    }

    jingle.play_jingle(common_stuff.savefile.options.sound_vol);

    // serif_white.generate(nicework_x, nicework_y, "nice work!", _nicework_text_sprites);

    if(levels[_sesh.get_level()][0] == 3)
    {
        //night level
        _common_stuff.acm.update_achievement("night");
    }    

}

void char_unlock_scene::save(){

    _common_stuff.save();

    _common_stuff.set_autosave_text_visible(true);
}

bn::optional<scene_type> char_unlock_scene::update(){
    bn::optional<scene_type> result;

    if(_a_button.visible())
    {
        _a_button_anim.update();
    }

    //you can skip this scene by pressing A.
    if(bn::keypad::a_pressed()){

            result = _next_scene;

        _common_stuff.set_autosave_text_visible(false);
    }

    return result;
}

}