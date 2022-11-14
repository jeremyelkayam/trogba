#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_knight.h"
#include "entity.h"
#include "session_info.h"
#include "constants.h"

namespace trog {
    class knight : public entity { 
        private:
            bn::fixed _speed;

            bn::fixed_point _direction;
 
            // bn::sprite_animate_action<3> _walkcycle;
            bool going_to_go_offscreen_x(bn::fixed_point pos, bn::fixed speed, bn::fixed_point direction);  

            bool going_to_go_offscreen_y(bn::fixed_point pos, bn::fixed speed, bn::fixed_point direction);  

        public:
            knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, bn::fixed_point direction);
            virtual void update() final;
    };
}