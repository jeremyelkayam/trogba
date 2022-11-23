#include "bonus_scene.h"
#include "bn_regular_bg_items_cottageinterior.h"
#include <bn_log.h>
#include <bn_sound_items.h>

namespace trog {

bonus_scene::bonus_scene(session_info &sesh) :
        _bg(bn::regular_bg_items::cottageinterior.create_bg(TROG_COTTAGEINTERIOR_BG_X, TROG_COTTAGEINTERIOR_BG_Y)),
        _exit(100,0,20,20),
        _trogdor(sesh, _dummy, false),
        _sesh(sesh) {
}


bn::optional<scene_type> bonus_scene::update(){
    bn::optional<scene_type> result;
    _trogdor.update();
    // move_background(_bg);

    //if you walk to the exit you must return to play scene
    if(_trogdor.get_hitbox().intersects(_exit)){
        BN_LOG("return to play scene");
        result = scene_type::PLAY;
    }
    return result;
}

}