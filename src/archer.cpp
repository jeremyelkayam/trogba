#include <bn_log.h>
#include "archer.h"

namespace trog { 

archer::archer(bn::fixed ycor, bool facing_left, common_stuff &common_stuff) : 
        entity(0, ycor, bn::fixed(0), bn::fixed(0), bn::sprite_items::archer.create_sprite(-300, ycor)),
        _facing_left(facing_left),
        _time_since_spawn(0),
        _common_stuff(common_stuff) {
    _sprite.set_horizontal_flip(facing_left);
    if(facing_left){
        _pos.set_x(110);
        _sprite.set_x(110);
    }else{
        _pos.set_x(-110);
        _sprite.set_x(-110);
    }
}

void archer::update(){
    entity::update();

    ++_time_since_spawn;
    if(TROG_ARCHER_DRAWBOW_WAITTIME < _time_since_spawn 
        && _time_since_spawn < TROG_ARCHER_SHOOT_WAITTIME){
        set_bow_drawn(true);
    }else if (TROG_ARCHER_SHOOT_WAITTIME == _time_since_spawn && !_arrow){
        set_bow_drawn(false);
        shoot();
    }else if(TROG_ARCHER_DISAPPEAR_WAITTIME < _time_since_spawn){
        _sprite.set_visible(false);
    }

    if(_arrow){
        _arrow->update();
    }
}

void archer::shoot(){
    bn::sound_items::shoot.play(_common_stuff.savefile.options.sound_vol);
    _arrow.emplace(_pos, _facing_left);
}

void archer::set_bow_drawn(bool drawn){
    if(drawn){
        _sprite.set_tiles(bn::sprite_items::archer.tiles_item(), 1);
    }else{
        _sprite.set_tiles(bn::sprite_items::archer.tiles_item(), 0);
    }
}

bool archer::remove_from_map(){
    if(_arrow) {
        return _arrow->out_of_bounds();
    }else if(!_arrow && TROG_ARCHER_DISAPPEAR_WAITTIME < _time_since_spawn){
        //at this point we've shot our arrow and it also got deleted
        // possibly from hitting something
        // so we need to despawn      
        return true;
    }else return false;
}

bn::fixed_rect archer::get_hitbox() const {
    if(_arrow && !_arrow->rotating()){
        return _arrow->get_hitbox();
    }else{
        //bogus rectangle that's way offscreen  
        return bn::fixed_rect(-500,-500, 0, 0);
    }
}

void archer::destroy_arrow() {
    _arrow.reset();
}

void archer::set_visible(const bool &visible){
    entity::set_visible(visible);
    if(_arrow){
        _arrow->set_visible(visible);
    }
}

void archer::stomp_on(bn::fixed_point foot_pos, uint8_t stomp_radius){
    if(_arrow){
        if(_common_stuff.euclidean_dist(foot_pos, _arrow->get_pos()) <= stomp_radius){
            _arrow->plink();
            _common_stuff.update_achievement("stompar");
        }
    }else{
        if(_common_stuff.euclidean_dist(foot_pos, get_pos()) <= stomp_radius){
            //for now they can just leave
            _time_since_spawn = TROG_ARCHER_DISAPPEAR_WAITTIME;
        }        
    }
}

}
