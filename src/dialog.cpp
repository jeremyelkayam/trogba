#include "dialog.h"
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "common_functions.h"
#include "small_fonts.h"


namespace trog {

dialog::dialog(const char *text) {  

    bn::sprite_text_generator white_generator = 
    bn::sprite_text_generator(small_font_white);
    white_generator.set_center_alignment();

    bn::vector<bn::string<64>, 3> lines = split_into_lines(text, 176);

    bn::fixed yoffset = -9 * lines.size() / 2;
    for(const bn::string<64> &line : lines)
    {
        white_generator.generate(0, yoffset, 
            line, _text_sprites);
        yoffset += 9;
    }

    white_generator.generate(0, 30, 
        "Press A to continue.", _text_sprites);
}

}