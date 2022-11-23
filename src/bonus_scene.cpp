#include "bonus_scene.h"
#include "bn_regular_bg_items_cottageinterior.h"
#include <bn_log.h>
#include <bn_sound_items.h>

namespace trog {

bonus_scene::bonus_scene(session_info &sesh) :
        _bg(bn::regular_bg_items::cottageinterior.create_bg(TROG_COTTAGEINTERIOR_BG_X, TROG_COTTAGEINTERIOR_BG_Y)),
        _sesh(sesh) {

}


bn::optional<scene_type> bonus_scene::update(){
    bn::optional<scene_type> result;

    return result;
}

}