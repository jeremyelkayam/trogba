#include <bn_log.h>

#include "troghammer.h"
#include "bn_sprite_items_troghammer.h"

namespace trog { 

troghammer::troghammer(int xcor, int ycor, bool facingRight, int level) :
    knight(xcor, ycor, facingRight),
    _waiting_time(120 SECONDS)
{
    //TODO: Make the troghammer bigger, he's not imposing enough
    _sprite = bn::sprite_items::troghammer.create_sprite(xcor, ycor);
    _sprite.set_horizontal_flip(facingRight);
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
}

void troghammer::update(){
    if(_waiting_time > 0){
        --_waiting_time;
        // BN_LOG("waiting time for troghammer", _waiting_time);
    }else{
        knight::update();
    }
}


}