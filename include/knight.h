#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_knight.h"
#include "entity.h"
#include "session_info.h"
#include "constants.h"
#include "bn_math.h"

namespace trog {
    class knight : public entity { 
        private:
            
            unsigned short _frames_moving;

            bn::fixed _speed;
            unsigned short _direction;
 
            // bn::sprite_animate_action<3> _walkcycle;
            bool going_to_go_offscreen(bn::fixed_rect hitbox, bn::fixed speed, bn::fixed direction);  

            bn::fixed_point unit_vector(bn::fixed angle){return bn::fixed_point(bn::degrees_cos(angle),bn::degrees_sin(angle));}
        public:
            knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, unsigned short direction);
            virtual void update() final;
    };
}