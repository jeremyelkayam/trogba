// big_text.h
// Purpose: represent the big-ass text that shows up sometimes

#pragma once
#include <bn_affine_bg_ptr.h>
#include <bn_affine_bg_actions.h>

#include "big_text.h"

namespace trog {
    class burninate_text : public big_text { 
        private:
            bn::affine_bg_ptr _fire;
            bn::affine_bg_scale_loop_action _scaler;
            unsigned short _timer;

        public:
            burninate_text(bn::sprite_text_generator &big_text_generator);
			virtual void update() final;
            virtual void set_visible(bool visible) final;
    };
}