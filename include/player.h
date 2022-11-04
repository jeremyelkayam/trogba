#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_fixed_rect.h>
#include "bn_sprite_items_player.h"

namespace trog {
    class player { 
        private:
            bn::fixed_point pos;
            bn::fixed_rect hitbox;
            const bn::fixed speed;
            bn::fixed direction;
 
            bn::sprite_ptr sprite;
            bn::sprite_animate_action<4> walkcycle;
            
            unsigned short trogmeter;
            unsigned int burninate_time;
            const int trogmeter_max = 10;
            const int burninate_length = 600; //temporarily this can be 10 seconds 
            void move();
            void check_collision();
        public:
            player();
            void update();
            bool burninating();
            void set_direction(bn::fixed new_direction) {direction = new_direction;};
    };
}