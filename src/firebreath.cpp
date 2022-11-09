#include <bn_math.h>
#include <bn_log.h>
#include "firebreath.h"
#include "bn_assert.h"

namespace trog {

firebreath::firebreath() : 
        entity(bn::fixed(0), bn::fixed(0), bn::fixed(20), bn::fixed(45), bn::sprite_items::firebreath.create_sprite(0, 0)),
        _burningflames(bn::create_sprite_animate_action_forever(
                    _sprite, 10, bn::sprite_items::firebreath.tiles_item(), 0, 1, 2, 3)){
    _sprite.set_z_order(FRONT_ZORDER);
    disable();

}

void firebreath::update(){
    entity::update();
    _burningflames.update();
}

void firebreath::enable(){
    _sprite.set_visible(true);
}

void firebreath::disable(){
    _sprite.set_visible(false);
}

bool firebreath::enabled(){
    return _sprite.visible();
}

}