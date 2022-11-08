#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_peasant.h"
#include "entity.h"
#include "enums.h"

namespace trog {
    class peasant : public entity { 
        private:
            const bn::fixed _maxdist;
            bn::fixed _currentdist;
            bn::fixed _speed;

            const unsigned short _waittime;
            unsigned short _time_waiting;

            const unsigned short _despawn_delay;
            unsigned short _time_dead;

            bn::fixed_point _direction;
            bool _onfire;
 
            bn::sprite_animate_action<2> _walkcycle;
            
            void check_boundary_collision();
        public:
            peasant(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed maxdist, direction direction);
            virtual void update() final;

            short stomp();
            void burninate();
            bool dead();

    };
}