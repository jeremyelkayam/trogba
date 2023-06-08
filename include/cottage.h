#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "enums.h"

namespace trog {
    class cottage : public entity { 
        private:
            direction _direction;

            bn::vector<bn::sprite_ptr,2> _bits;
            bn::optional<bn::sprite_ptr> _flames;
            bn::optional<bn::sprite_animate_action<4>> _burningflames;
            uint8_t _time_burning;
        public:
            cottage(bn::fixed xcor, bn::fixed ycor, direction direction);
            virtual void update() final;
            bool burninate();
            bool burninated() {return _time_burning > (120 / 2);}
            direction get_direction() {return _direction;}
    };
}