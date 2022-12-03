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
            
            unsigned short _frames_moving;

            bn::fixed _speed;
            unsigned short _direction;
 
            bn::sprite_animate_action<4> _walkcycle;
        public:
            knight(bn::fixed xcor, bn::fixed ycor, bn::fixed speed, unsigned short direction);
            virtual void update() final;
    };
}