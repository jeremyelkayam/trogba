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

    _money.emplace_front(-80, 40);
    _money.emplace_front(-40, 20);
    _money.emplace_front(0, 60);
    _money.emplace_front(40, 40);
    _money.emplace_front(0, 0);

}


bn::optional<scene_type> bonus_scene::update(){
    bn::optional<scene_type> result;
    _trogdor.update();

    //if you walk to the exit you must return to play scene
    if(_trogdor.get_hitbox().intersects(_exit)){
        BN_LOG("return to play scene");
        result = scene_type::PLAY;
    }


    for(moneybag &bag : _money){
        if(_trogdor.get_hitbox().intersects(bag.get_hitbox())){
            //todo refactor this into trogdor
            _sesh.score += TROG_MONEYBAG_POINTS;
            bn::sound_items::goldget.play(1);
            bag.collect();
        }
    }

    _money.remove_if(moneybag_deletable);

    
    return result;
}

}