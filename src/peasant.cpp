#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>

#include "peasant.h"
#include "entity.h"
#include "sb_commentary.h"
#include "bn_sprite_items_peasantdead.h"
#include "bn_sprite_items_alert_icon.h"

namespace trog {

peasant::peasant(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed maxdist, cottage &home) : 
        freezable(xcor, ycor, TROG_PEASANT_WIDTH, TROG_PEASANT_HEIGHT, bn::sprite_items::peasant.create_sprite(xcor, ycor)),
        _home(home),
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
                    _sprite, 12, bn::sprite_items::peasant.tiles_item(), 0, 1)),
        _alert_icon(bn::sprite_items::alert_icon.create_sprite(0,0)){
    _sprite.set_z_order(MID_ZORDER);
    _alert_icon.set_z_order(MID_ZORDER);
    _alert_icon.set_visible(false);

    switch(home.get_direction()) {
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
    //no two peasants are not onfire(). awwww 
    BN_ASSERT(!dead() && !onfire());

    // set on fire
    _onfire = true;
    _freeze_timer = 0;
    run_to_house();

    // change animation to flaming
    set_sprite_ablaze();
}

void peasant::run_to_house(){
    // run fast. and flip your direction if you weren't already returning
    if(_returning){
        _speed = _speed*2;
    }else{
        _speed = _speed*-2;
    }
    _currentdist+=_speed;
}

void peasant::set_sprite_ablaze(){
    _walkcycle = bn::create_sprite_animate_action_forever(
                    _sprite, 3, bn::sprite_items::peasant.tiles_item(), 2, 3);
    _sprite.set_tiles(bn::sprite_items::peasant.tiles_item(), 2);
}

void peasant::update(){
    entity::update();
    _alert_icon.set_x(_pos.x() + 2);
    _alert_icon.set_y(_pos.y() - 16);

    if(!_freeze_timer){
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
            
        }else if(!_returning){
            _time_waiting++;
        }
    }else{
        update_freeze();

        //this will trigger if the peasant just unfroze
        if(_freeze_timer == 0) alert();
    }
}

void peasant::squish(){
    //cannot squish a peasant that is already stomped.
    if(_time_dead == 0){
        _speed = bn::fixed(0);
        _time_dead=1;
        _sprite.set_item(bn::sprite_items::peasantdead);
        _sprite.set_y(_sprite.y() + 3);
        _pos.set_y(_pos.y() + 3);
        _freeze_timer = 0;
        _alert_icon.set_visible(false);
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
