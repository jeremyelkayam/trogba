#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_peasant.h"
#include "entity.h"
#include "enums.h"
#include "session_info.h"
#include "constants.h"

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
            bool _returning;

            //idk why we need this, but there's this weird thing where peasants
            //will appear at the center of the map when they spawn,
            //and then immediately warp to their correct location and it looks bad.
            //the only way i have figured out how to fix it is to set them
            //invisible in the constructor, but make them visible once they spawn
            bool _just_spawned;
 
            bn::sprite_animate_action<2> _walkcycle;
            
        public:
            peasant(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed maxdist, direction direction);
            virtual void update() final;

            void stomp();
            void burninate();
            bool dead(){return _time_dead > 0;}
            bool remove_from_map();
            bool onfire(){return _onfire;}

    };
}