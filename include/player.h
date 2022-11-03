#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_player.h"

namespace trog {
    class player { 
        private:
            bn::sprite_ptr sprite = bn::sprite_items::player.create_sprite(0, 0);
            bn::sprite_animate_action<4> walkcycle = bn::create_sprite_animate_action_forever(
                        sprite, 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3);
            unsigned short trogmeter;
            unsigned int burninate_time;
            const int trogmeter_max = 10;
            const int burninate_length = 600; //temporarily this can be 10 seconds 
        public:
            player();
            void update();
            bool burninating();
    };
}