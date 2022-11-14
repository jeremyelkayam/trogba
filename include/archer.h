#pragma once
#include <bn_sprite_actions.h>
#include <bn_sound_items.h>
#include "bn_sprite_items_archer.h"
#include "entity.h"
#include "constants.h"
#include "arrow.h"

namespace trog {
    class archer : public entity { 
        private:
            bool _facing_left;
            unsigned short _time_since_spawn;
            void shoot();
            bn::optional<arrow> _arrow;
 
        public:
            archer(bn::fixed ycor, bool facing_left);
            virtual void update() final;
            bool remove_from_map();
    };
}