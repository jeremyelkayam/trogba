#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_player.h"
#include "entity.h"
#include "cottage.h"

namespace trog {
    class player : public entity { 
        private:
            const bn::fixed speed;
 
            bn::sprite_animate_action<4> walkcycle;
            
            unsigned short trogmeter;
            unsigned int burninate_time;
            const int trogmeter_max = 10;
            const int burninate_length = 600; //temporarily this can be 10 seconds 
            void move();
            void check_boundary_collision();
        public:
            player();
            virtual void update() final;
            bool burninating();
            void check_cottage_collision(cottage &cottage);

    };
}