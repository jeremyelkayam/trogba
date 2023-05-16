#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "bn_regular_bg_items_titlebg.h"
#include "bn_sprite_items_titlegraphic.h"
#include "bn_sprite_items_versionlabel.h"
#include "title_scene.h"

namespace trog {

title_scene::title_scene(common_stuff &common_stuff) : 
        _titlebg(bn::regular_bg_items::titlebg.create_bg(TROG_TITLE_BG_X, TROG_TITLE_BG_Y)),
        _version_label(bn::sprite_items::versionlabel.create_sprite(-104, 77)),
        _frame_counter(0) {

    _version_label.set_item(bn::sprite_items::versionlabel, 2);

    bn::sound_items::themesong.play(common_stuff.savefile.options.music_vol);

    for(int z = 0; z < 4 ; ++z){
        _title_sprites.push_back(bn::sprite_items::titlegraphic.create_sprite(TROG_TITLE_TEXT_X + 64*z, TROG_TITLE_TEXT_Y, z));
    }
}

bn::optional<scene_type> title_scene::update(){
    bn::optional<scene_type> result;
    _frame_counter++;
    // BN_LOG(frame_counter);

    if(_frame_counter > 440 || bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        result = scene_type::MENU;
    }
    
    return result;
}

}