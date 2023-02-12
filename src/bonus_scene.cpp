#include "bonus_scene.h"
#include "bn_regular_bg_items_cottageinterior.h"
#include "bn_sprite_items_debug_box.h"
#include <bn_log.h>

namespace trog {

bonus_scene::bonus_scene(session_info &sesh) :
        _bg(bn::regular_bg_items::cottageinterior.create_bg(TROG_COTTAGEINTERIOR_BG_X, TROG_COTTAGEINTERIOR_BG_Y)),
        _exit(110,0,20,20),
        _trogdor(80,10, sesh, false),
        _sesh(sesh) {
    _trogdor.set_horizontal_flip(true);

    //todo refactor this into an array of pairs with a for loop. would be cleaner
    _money.emplace_front(-90, 30);
    _money.emplace_front(-50, 0);
    _money.emplace_front(-50, 60);
    _money.emplace_front(-10, 30);
    _money.emplace_front(30, 0);
    _money.emplace_front(30, 60);
    _money.emplace_front(70, 30);


    _wall_hitboxes.emplace_front(0,-50,240,60);
    _wall_hitboxes.emplace_front(-110,-10,20,40);
}


bn::optional<scene_type> bonus_scene::update(){
    bn::optional<scene_type> result;
    _trogdor.update();

    //if you walk to the exit you must return to play scene
    if(_trogdor.get_hitbox().intersects(_exit) && _money.empty()){
        BN_LOG("return to play scene");
        result = scene_type::PLAY;
    }

    for(moneybag &bag : _money){
        if(_trogdor.get_hitbox().intersects(bag.get_hitbox())){
            //todo refactor this into trogdor
            _sesh.score(TROG_MONEYBAG_POINTS);
            bag.collect();
        }
    }
    for(bn::fixed_rect &wall : _wall_hitboxes) {
        _trogdor.handle_wall_collision(wall);
    }

    _money.remove_if(moneybag_deletable);

    return result;
}

}