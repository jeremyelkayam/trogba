// king.h

#pragma once
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_king.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class king : public entity { 
        protected:
            bn::sprite_ptr _right_sprite;

        public:
            king(int xcor, int ycor);
			virtual void update() final;
            virtual void update_anim() final;
    };
}