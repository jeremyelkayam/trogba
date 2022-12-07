#include "big_text.h"

namespace trog { 



big_text::big_text(bn::sprite_text_generator &big_text_generator, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette){
    big_text_generator.set_center_alignment();
    big_text_generator.set_palette_item(palette);
    big_text_generator.generate(x, y, text, _text_sprites);
}

}