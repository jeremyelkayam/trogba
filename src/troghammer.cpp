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
                    _sprite, 20, bn::sprite_items::troghammer.tiles_item(), 0, 0, 0, 0);
}

}