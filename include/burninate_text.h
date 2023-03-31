// big_text.h
// Purpose: represent the big-ass text that shows up sometimes

#pragma once
#include <bn_affine_bg_ptr.h>
#include <bn_affine_bg_actions.h>

#include "big_text.h"

namespace trog {
    class burninate_text : public big_text { 
        private:

        public:
            burninate_text();
			virtual void update() final;
            virtual void set_visible(bool visible) final;
    };
}