// bubs.h
// Purpose: represent Bubs' as he lets it all hang out

#pragma once
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_bubs.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class bubs : public entity { 
        protected:
            bn::sprite_animate_action<4> _anim;

        public:
            bubs(int xcor, int ycor);
			virtual void update() final;
            virtual void update_anim() final;
    };
}