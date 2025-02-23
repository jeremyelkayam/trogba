#include "bonus_scene.h"
#include "bn_regular_bg_items_cottageinterior.h"
#include "sucks.h"
#include "wormdingler.h"
#include "chiaroscuro.h"

#include <bn_log.h>
#include <bn_sound_items.h>

namespace trog {

bonus_scene::bonus_scene(session_info &sesh, common_stuff &common_stuff) :
        _bg(bn::regular_bg_items::cottageinterior.create_bg(TROG_COTTAGEINTERIOR_BG_X, TROG_COTTAGEINTERIOR_BG_Y)),
        _exit(110,0,20,20),
        _player(new trogdor(80,10, sesh, false, common_stuff)),
        _common_stuff(common_stuff),
        _sesh(sesh) {

            //TODO this sucks make it better
    switch(_sesh.get_dragon()){
        case dragon::TROGDOR:
            _player.reset(new trogdor(80,10, sesh, false, common_stuff));
        break;
        case dragon::SUCKS:
            _player.reset(new sucks(80,10, sesh, false, common_stuff));
        break;
        case dragon::WORMDINGLER:
            _player.reset(new wormdingler(80,10, sesh, false, common_stuff));
        break;
        case dragon::CHIAROSCURO:
            _player.reset(new chiaroscuro(80,10, sesh, false, common_stuff));
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

    //todo: refactor this into commentary maybe
    if(common_stuff.percent_chance(30) && 
        ((_sesh.get_dragon() == dragon::TROGDOR) || 
         (_sesh.get_dragon() == dragon::SUCKS) )){
        bn::sound_items::sb_roomierinside.play(
            common_stuff.savefile.options.voice_vol);
    }

    // common_stuff.savefile.session.can_visit_treasure_hut = false;
    common_stuff.acm.update_achievement("onehut", 0);


    //the three treasure hut levels
    // TODO - should iterate through the level_data
    if(_sesh.get_level() % 32 == 7)
    {
        common_stuff.acm.update_achievement("allhuts", 0);
    }
    else if(_sesh.get_level() % 32 == 11)
    {
        common_stuff.acm.update_achievement("allhuts", 1);
    }
    else if(_sesh.get_level() % 32 == 16)
    {
        common_stuff.acm.update_achievement("allhuts", 2);
    }

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
            bn::sound_items::goldget.play(_common_stuff.savefile.options.sound_vol);
            bag.collect();
            _common_stuff.savefile.stats.treasure_collected++;
        }
    }
    for(bn::fixed_rect &wall : _wall_hitboxes) {
        _player->handle_wall_collision(wall);
    }

    _money.remove_if(moneybag_deletable);

    return result;
}

}