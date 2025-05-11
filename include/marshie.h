// marshie.h

#pragma once
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_marshie.h"
#include "entity.h"

#include "constants.h"

namespace trog {
    class marshie : public entity { 
        protected:
            bn::sprite_animate_action<4> _anim;

            unsigned int _timer = 0;
            static constexpr const unsigned int FIG8_TIME = 120;

        public:
            marshie(int xcor, int ycor);
			virtual void update() final;
            virtual void update_anim() final;
    };
}