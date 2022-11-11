#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "constants.h"
#include "enums.h"

namespace trog {
    class cottage : public entity { 
        private:
            direction _direction;

            bn::sprite_ptr _flames;
            bn::sprite_animate_action<4> _burningflames;
            int _time_burning;
        public:
            cottage(bn::fixed xcor, bn::fixed ycor, direction direction);
            virtual void update() final;
            void burninate();
            bool burninated() {return _time_burning > (TROG_COTTAGEFIRE_TIME / 2);}
            direction get_direction() {return _direction;}
    };
}