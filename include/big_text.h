// big_text.h
// Purpose: represent the big-ass text that shows up sometimes

#pragma once
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_string.h>

#include "constants.h"

namespace trog {
    class big_text { 
        private:

            bn::vector<bn::sprite_ptr, 64>_text_sprites;

        public:
            big_text(bn::sprite_text_generator &big_text_generator, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette);
			void update() {return;}
    };
}