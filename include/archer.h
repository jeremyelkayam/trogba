/**  
 * archer.h
 * Author: Jeremy Elkayam
 * 
 * Purpose: Represent an archer 
 * 
 */

#pragma once
#include <bn_sprite_actions.h>
#include <bn_sound_items.h>
#include "bn_sprite_items_archer.h"
#include "entity.h"
#include "constants.h"
#include "common_stuff.h"
#include "arrow.h"

namespace trog {
    class archer : public entity {
        private:
            bool _facing_left;
            unsigned short _time_since_spawn;
            void shoot();
            bn::optional<arrow> _arrow;
            common_stuff &_common_stuff;

        public:
            archer(bn::fixed ycor, bool facing_left, common_stuff &common_stuff);
            virtual void update() final;

            /**
             * Once an archer takes his shot, he can be removed from the game.
             * This function returns true if the archer has shot an arrow
             * and the arrow has either gone offscreen or hit a player.
             * 
             * @return  whether or not this archer should be removed from memory.
             */
            bool remove_from_map();

            /**
             * Override from entity.
             * @return  the hitbox of the arrow shot by this archer.
             */
            virtual bn::fixed_rect get_hitbox() const final;

            /**
             * Destroy the arrow owned by this archer if it exists.
            */
            void destroy_arrow();
            void set_bow_drawn(bool drawn);
            virtual void set_visible(const bool &visible) override final;

            void stomp_on(bn::fixed_point foot_pos, uint8_t stomp_radius);
    };
}