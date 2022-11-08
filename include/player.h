#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_player.h"
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "constants.h"

namespace trog {
    class player : public entity { 
        private:
            const bn::fixed speed;
 
            bn::sprite_animate_action<4> walkcycle;
            
            unsigned short trogmeter;
            unsigned int burninate_time;
            const unsigned short trogmeter_max = TROG_TROGMETER_MAX;
            const int burninate_length = TROG_BURNINATE_TIME; //temporarily this can be 10 seconds 
            void move();
            void check_boundary_collision();
        public:
            player();
            virtual void update() final;
            bool burninating();
            void check_cottage_collision(cottage &cottage);
            void check_peasant_collision(peasant &peasant);
            unsigned short get_trogmeter_max(){return trogmeter_max;}
            unsigned short get_trogmeter(){return trogmeter;}

    };
}