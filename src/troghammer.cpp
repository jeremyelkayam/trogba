#include <bn_log.h>

#include "troghammer.h"
#include "bn_sprite_items_troghammer.h"

namespace trog { 

//TODO: Make a saved_data class/struct so that we can save troghammer status

troghammer::troghammer(bool facingRight, int level) :
    knight(-100, -60, facingRight),
    _waiting_time(120 SECONDS)
{
    //TODO: Make the troghammer bigger, he's not imposing enough
    _sprite = bn::sprite_items::troghammer.create_sprite(_pos.x(), _pos.y());
    _sprite.set_horizontal_flip(facingRight);
    _sprite.set_scale(0.25);
    _sprite.put_below();
    _walkcycle = bn::create_sprite_animate_action_forever(
                    _sprite, (20 / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer(), bn::sprite_items::troghammer.tiles_item(), 1, 3, 4, 3);
    _speed *= TROG_HAMMER_SPEEDUP_FACTOR;
    _cycletime = (bn::fixed(_cycletime) / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer();

    //hitboxes are 20% bigger to make it harder
    _hitbox.set_width(TROG_HAMMER_WIDTH);
    _hitbox.set_height(TROG_HAMMER_HEIGHT);

    //for every 10 levels the waiting time gets 10% lower.
    _waiting_time -= _waiting_time * (level / 9) * 0.1;
 
    if(level > 90) _waiting_time = 0;
    _initial_waiting_time = _waiting_time;
}

void troghammer::update(){

    if(_waiting_time == _initial_waiting_time / 2) {
        set_x(TROG_COUNTRYSIDE_RIGHT_BOUND);
        _sprite.set_scale(0.5);
        _sprite.set_horizontal_flip(!_sprite.horizontal_flip());
        _speed = -_speed;
    }

     if(_waiting_time == 1){
        set_y(TROG_COUNTRYSIDE_TOP_BOUND + 10);
        set_x(0);
        _sprite.set_scale(1);
        _speed = -_speed;
     }

    if(_waiting_time > 0){
        --_waiting_time;
        BN_LOG("waiting time for troghammer", _waiting_time);
        set_x(_pos.x() + _speed * _sprite.vertical_scale());
        if(_pos.x().floor_integer() < 120 && 
            _pos.x().floor_integer() > -120){
            set_y(daytime_path[_pos.x().floor_integer() + 120]);
        }
        _walkcycle.update();
    }else{
        knight::update();
    }
}


}