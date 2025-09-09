#include "dialog.h"
#include <bn_sprite_text_generator.h>
#include <bn_string.h>
#include "common_functions.h"
#include "small_fonts.h"
#include "bn_regular_bg_items_dialog.h"


namespace trog {

dialog::dialog(const char *text) : 
    _box(bn::regular_bg_items::dialog.create_bg(0,0)) {  

    bn::sprite_text_generator white_generator = 
    bn::sprite_text_generator(small_font_white);
    white_generator.set_center_alignment();
    white_generator.set_bg_priority(0);
    white_generator.set_z_order(5);

    bn::vector<bn::string<64>, 6> lines = split_into_lines(text, 165);

    bn::fixed yoffset = -11 * lines.size() / 2;
    for(const bn::string<64> &line : lines)
    {
        white_generator.generate(0, yoffset, 
            line, _text_sprites);
        yoffset += 11;
    }

    white_generator.generate(0, yoffset + 20, 
        "Press A to continue.", _text_sprites);
    
    _box.set_priority(0);
    

        
}

}