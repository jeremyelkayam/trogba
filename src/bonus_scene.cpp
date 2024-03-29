#include "bonus_scene.h"
#include "bn_regular_bg_items_cottageinterior.h"
#include "bn_sprite_items_debug_box.h"
#include "sucks.h"

#include <bn_log.h>
#include <bn_sound_items.h>

namespace trog {

bonus_scene::bonus_scene(session_info &sesh, common_stuff &common_stuff) :
        _bg(bn::regular_bg_items::cottageinterior.create_bg(TROG_COTTAGEINTERIOR_BG_X, TROG_COTTAGEINTERIOR_BG_Y)),
        _exit(110,0,20,20),
        _player(new trogdor(80,10, sesh, false, common_stuff)),
        _sesh(sesh) {

    switch(_sesh.get_dragon()){
        case dragon::TROGDOR:
            _player.reset(new trogdor(80,10, sesh, false, common_stuff));
        break;
        case dragon::SUCKS:
            _player.reset(new sucks(80,10, sesh, false, common_stuff));
        break;
        default:
            BN_ERROR("Invalid dragon type found in session info");
        break;
    }
    _player->set_horizontal_flip(true);

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
    _player->update();

    //if you walk to the exit you must return to play scene
    if(_player->get_hitbox().intersects(_exit) && _money.empty()){
        BN_LOG("return to play scene");
        result = scene_type::PLAY;
    }

    for(moneybag &bag : _money){
        if(_player->get_hitbox().intersects(bag.get_hitbox())){
            //todo refactor this into trogdor
            _sesh.score(TROG_MONEYBAG_POINTS);
            bn::sound_items::goldget.play(1);
            bag.collect();
        }
    }
    for(bn::fixed_rect &wall : _wall_hitboxes) {
        _player->handle_wall_collision(wall);
    }

    _money.remove_if(moneybag_deletable);

    return result;
}

}