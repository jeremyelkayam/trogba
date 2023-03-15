#pragma once
#include "bn_sprite_items_arrow.h"
#include "entity.h"

namespace trog {
    class arrow : public entity { 
        private:
            bool _facing_left;
            bn::fixed _speed;

        public:
            arrow(bn::fixed_point pos, bool facing_left);
            virtual void update() final;
    };
}