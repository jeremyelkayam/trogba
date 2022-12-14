#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_log.h>

#include "bn_regular_bg_items_videlectrix.h"
#include "bn_regular_bg_items_videclectix.h"
#include "logo_scene.h"

namespace trog {

logo_scene::logo_scene() : 
        _bg(bn::regular_bg_items::videlectrix.create_bg(0, -40)),
        _frame_counter(0),
        _easter_egg(false) {

    bn::sound_items::videlectrix_jingle.play(TROG_DEFAULT_VOLUME);
}

bn::optional<scene_type> logo_scene::update(){
    bn::optional<scene_type> result;
    _frame_counter++;
    // BN_LOG(frame_counter);

    if(bn::keypad::select_pressed()) { //KICK!
        if(_easter_egg){
            _bg.set_item(bn::regular_bg_items::videlectrix);
        }else{
            _bg.set_item(bn::regular_bg_items::videclectix);
        }
        _easter_egg = !_easter_egg;
        _bg.set_mosaic_enabled(true);
        bn::bgs_mosaic::set_stretch(1);
        _mosaic_action = bn::bgs_mosaic_stretch_to_action(20, 0);
    }

    if(_mosaic_action && !_mosaic_action->done()){
        _mosaic_action->update();
    }

    if(_frame_counter > 360 || bn::keypad::start_pressed() || bn::keypad::a_pressed()){
        result = scene_type::TITLE;
    }
    
    return result;
}

}