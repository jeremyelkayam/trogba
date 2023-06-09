// big_text.h
// Purpose: represent the big-ass text that shows up sometimes

#pragma once
#include <bn_sprite_actions.h>
#include "big_text.h"

namespace trog {
    class burninate_text : public big_text { 
        private:
            // bn::sprite_ptr _fire;
            // bn::optional<bn::sprite_scale_loop_action> _fire_scale;
            // bn::sprite_move_loop_action _fire_move;

        public:
            burninate_text(bn::sprite_text_generator &generator);
			virtual void update() final;
            virtual void set_visible(bool visible) final;
    };
}