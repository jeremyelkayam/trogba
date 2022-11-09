#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_firebreath.h"
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "constants.h"

namespace trog {
    class firebreath : public entity { 
        private:
 
            bn::sprite_animate_action<4> _burningflames;
            
        public:
            firebreath();
            virtual void update() final;
            void check_cottage_collision(cottage &cottage);
            void check_peasant_collision(peasant &peasant);
            void enable();
            void disable();
            bool enabled();

            void set_horizontal_flip(bool flip) { _sprite.set_horizontal_flip(flip);}

    };
}