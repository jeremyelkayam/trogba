#include <bn_log.h>
#include "moneybag.h"
#include "bn_sprite_items_moneybag.h"
#include "constants.h"

namespace trog { 

moneybag::moneybag(bn::fixed xcor, bn::fixed ycor) : 
        entity(xcor, ycor, TROG_MONEYBAG_WIDTH, TROG_MONEYBAG_HEIGHT, bn::sprite_items::moneybag.create_sprite(xcor, ycor)),
        collected(false)
        {
    _sprite.set_z_order(BACK_ZORDER);
}


void moneybag::update(){
    entity::update();
}
void moneybag::collect(){
    collected = true;
}

}

