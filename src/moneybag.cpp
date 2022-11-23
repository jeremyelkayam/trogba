#include <bn_log.h>
#include "arrow.h"

namespace trog { 

arrow::arrow(bn::fixed_point pos, bool facing_left) : 
        entity(pos.x(), pos.y(), TROG_ARROW_WIDTH, TROG_ARROW_HEIGHT, bn::sprite_items::moneybag.create_sprite(pos)),
        {

}


void arrow::update(){
    entity::update();
}

}

