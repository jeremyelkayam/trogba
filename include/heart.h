// strongbad.h
// Purpose: represent that heart from the one cutscene

#pragma once
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_heart.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class heart : public entity { 
        protected:
            bn::optional<bn::sprite_animate_action<4>> _anim;

            
        public:
            heart(int xcor, int ycor);
			virtual void update() final;
            virtual void update_anim() final;
            void dissipate();
    };
}