#include "wormdingler.h"
#include "bn_sprite_items_wormdingler.h"
#include "bn_sprite_items_wormdingler_tongue.h"
#include <bn_math.h>

namespace trog { 

    //word mingler
    // haha get it

wormdingler::wormdingler(bn::fixed xcor, bn::fixed ycor, session_info &sesh, bool iframes, common_stuff &common_stuff, uint8_t initial_trogmeter) : 
    player(xcor, ycor, TROG_WORM_WIDTH, TROG_WORM_HEIGHT, TROG_WORM_SPEED, 
        bn::fixed_point(TROG_FIREBREATH_XOFFSET, TROG_FIREBREATH_YOFFSET), sesh, 
        iframes, bn::sprite_items::wormdingler, 5, common_stuff, initial_trogmeter), 
    _walkcycle(bn::create_sprite_animate_action_forever(_sprite, 3, 
            bn::sprite_items::wormdingler.tiles_item(), 0, 1, 2, 3, 4, 3, 2, 1)) {
}


void wormdingler::update_anim(){
    entity::update_anim();
    if((_move_action && !_move_action->done()) || _move_by_action || _update_anim_when_not_moving){
        _walkcycle.update();
    }
    update_firebreath();
}

void wormdingler::update(){
    player::update();
    if(!dead() && any_dpad_input()){
        _walkcycle.update();
    }
}

tongue::tongue(bn::fixed_point pos, bool facing_left) : 
    entity(pos.x(), 
        pos.y(), 
        13, 
        3, 
        bn::sprite_items::wormdingler_tongue.create_sprite(0, 0)),
    _retracting(false)
{
    _sprite.set_horizontal_flip(facing_left);
}

void tongue::update(){
    bn::fixed end = _sprite.x() + 4;
    bn::fixed begin = _pos.x();

    bn::fixed width = bn::abs(end - begin);
    bn::fixed xcor = (end + begin) * bn::fixed(0.5);

    _hitbox.set_width(width);
    _hitbox.set_x(xcor);
}

void tongue::retract()
{
    _retracting = true;
}

}