#pragma once
#include <bn_sprite_actions.h>
#include "bn_sprite_items_arrow.h"
#include "entity.h"
#include "constants.h"

namespace trog {
    class arrow : public entity { 
        private:
            bool _facing_left;
            bn::fixed _speed;
            bool _rotating;
            unsigned short _plink_timer;

        public:
            arrow(bn::fixed_point pos, bool facing_left);
            virtual void update() final;
            void update_anim();
            void rotate() {_rotating = true;}
            void plink();
    };
}