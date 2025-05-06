// ratherdashing.h

#pragma once
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_ratherdashing.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class ratherdashing : public entity { 
        protected:
            bn::sprite_animate_action<4> _anim;

        public:
            ratherdashing(int xcor, int ycor);
			virtual void update() final;
            virtual void update_anim() final;
    };
}