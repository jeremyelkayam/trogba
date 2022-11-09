#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "entity.h"
#include "enums.h"

namespace trog {
    class cottage : public entity { 
        private:
            bool _burninated;
            direction _direction;
        public:
            cottage(bn::fixed xcor, bn::fixed ycor, direction direction);
            virtual void update() final;
    };
}