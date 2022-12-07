// big_text.h
// Purpose: represent the big-ass text that shows up sometimes

#pragma once

#include "big_text.h"

namespace trog {
    class bloody_text : public big_text { 
        private:

        public:
            bloody_text(bn::sprite_text_generator &big_text_generator, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette);
			virtual void update() final;
            virtual void set_visible(bool visible) final;
    };
}