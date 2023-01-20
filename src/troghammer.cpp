#include <bn_log.h>

#include "troghammer.h"
#include "bn_sprite_items_troghammer.h"

namespace trog { 

troghammer::troghammer(int xcor, int ycor, bool facingRight) :
    knight(xcor, ycor, facingRight)
{
    _sprite = bn::sprite_items::troghammer.create_sprite(xcor, ycor);
    _sprite.set_horizontal_flip(facingRight);
    _walkcycle = bn::create_sprite_animate_action_forever(
                    _sprite, (20 / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer(), bn::sprite_items::troghammer.tiles_item(), 0, 3, 2, 3);
    _speed *= TROG_HAMMER_SPEEDUP_FACTOR;
    _cycletime = (bn::fixed(_cycletime) / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer();
    BN_LOG("troghammer speed ", _speed);

    //hitboxes are 20% bigger to make it harder
    _hitbox.set_width(_hitbox.width() * 1.2);
    _hitbox.set_height(_hitbox.height() * 1.2);
}

}