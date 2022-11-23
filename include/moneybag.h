#pragma once
#include <bn_sprite_actions.h>
#include "entity.h"

namespace trog {
    class moneybag : public entity { 
        private:

        public:
            moneybag(bn::fixed xcor, bn::fixed ycor);
            virtual void update() final;
    };
}