#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_trog.h"

class Player { 
    private:
        bn::sprite_ptr sprite = bn::sprite_items::trog.create_sprite(0, 0);
        bn::sprite_animate_action<4> walkcycle = bn::create_sprite_animate_action_forever(
                    sprite, 5, bn::sprite_items::trog.tiles_item(), 0, 1, 2, 3);
    public:
        Player();
        void update();
};