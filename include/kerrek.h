// knight.h
// Purpose: represent the knight hazards present on every level.
// Rotation logic adapted from the Trogdor Reburninated Knight class by Mips96

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_kerrek.h"
#include "bn_sprite_items_kerrek_smoked.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class kerrek : public entity { 
        protected:

            bn::optional<bn::sprite_animate_action<7>> _burninate;
            bn::sprite_animate_action<3> _reveal_roast;
            
            unsigned short _drop_timer, _squish_timer;

        public:
            kerrek(bn::fixed xcor, bn::fixed ycor);
			virtual void update() final;
            virtual void update_anim() final;
            void burninate();
            // virtual void update_anim() final;
    };
}