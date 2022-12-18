#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>
#include "peasant.h"
#include "entity.h"
namespace trog {

peasant::peasant(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed maxdist, direction direction) : 
        entity(xcor, ycor, bn::fixed(8), bn::fixed(18), bn::sprite_items::peasant.create_sprite(xcor, ycor)),
        _maxdist(maxdist),
        _currentdist(bn::fixed(0)), 
        _speed(speed),
        _waittime(180),// 3 seconds for now but can change
        _time_waiting(0),
        _despawn_delay(60),
        _time_dead(0),
        _onfire(false),
        _returning(false),
        _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 12, bn::sprite_items::peasant.tiles_item(), 0, 1)){
    _sprite.set_z_order(MID_ZORDER);

    switch(direction) {
        case direction::UP:
            _direction = bn::fixed_point(0,-1);
        break;
        case direction::DOWN:
            _direction = bn::fixed_point(0,1);
        break;
        case direction::LEFT:
            _direction = bn::fixed_point(-1,0);
        break;
        case direction::RIGHT:
            _direction = bn::fixed_point(1,0);
        break;
        default:
            BN_ASSERT(false, "bruh idk wtf you gave the peasant but it aint a direction");
        break;
    }

}

void peasant::burninate(){
    if(!dead() && !_onfire){
        // set on fire
        _onfire = true;
        
        // run fast the other way 
        if(_returning){
            _speed = _speed*2;
        }else{
            _speed = _speed*-2;
        }
        _currentdist+=_speed;

        // change animation to flaming
        set_sprite_ablaze();
        bn::sound_items::peasantscream.play(TROG_DEFAULT_VOLUME);
    }
}

void peasant::set_sprite_ablaze(){
    _walkcycle = bn::create_sprite_animate_action_forever(
                    _sprite, 3, bn::sprite_items::peasant.tiles_item(), 3, 4);
}

void peasant::update(){
    entity::update();
    if(_time_dead == 1){
       bn::sound_items::stomp.play(TROG_DEFAULT_VOLUME);
    }
    

    if(_time_waiting == _waittime){
        _speed=-_speed;
        _currentdist+=_speed;
        _returning = true;

        //reset time waiting, probably should refactor this out later but w/e
        _time_waiting = 0;
    }

    if(_time_dead == 0 && _currentdist < _maxdist && _time_waiting < _waittime){

        _pos+=_direction.multiplication(_speed);

        _currentdist+=_speed;
        _walkcycle.update();
    }else if(_time_dead > 0){
        _time_dead++;
        
    }else {
        // BN_LOG("waiting", _time_waiting, " of ", _waittime);
        _time_waiting++;
    }
}

void peasant::stomp(){
    //cannot stomp a peasant that is already stomped.
    if(_time_dead == 0){
        _speed = bn::fixed(0);
        _time_dead=1;
        _sprite.set_tiles(bn::sprite_items::peasant.tiles_item(), 2);
    }
}

bool peasant::remove_from_map(){
    //dead peasants should be removed and despawned 
    //peasants despawn 1s after being stomped, or after reentering their house
    return (_time_dead >= _despawn_delay) || (_currentdist < 0);
}

void peasant::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action){
        _walkcycle.update();
    }
    if(_update_anim_when_not_moving) _walkcycle.update();
}

}
