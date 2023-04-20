#pragma once
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include "enums.h"
#include "entity.h"

namespace trog { 
    class tutorial_arrow : public entity {
        private:
            uint8_t _timer;
            direction _dir;
            bn::fixed initial_pos;

        public:
            tutorial_arrow(bn::fixed xcor, bn::fixed ycor, direction dir);
            
            virtual void update();
            direction get_direction(){return _dir;}
    };
}