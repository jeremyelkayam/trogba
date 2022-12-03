#pragma once
#include <bn_sprite_actions.h>
#include <bn_sound_items.h>
#include "bn_sprite_items_archer.h"
#include "bn_sprite_items_archer_bowdrawn.h"
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
            virtual bn::fixed_rect get_hitbox() final;
            void destroy_arrow();
            void set_visible(bool visible);
    };
}