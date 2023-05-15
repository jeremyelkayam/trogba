#include <bn_keypad.h>
#include <bn_string.h>

#include "bn_regular_bg_items_hi_scores_bg.h"
#include "options_scene.h"
#include "constants.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"

namespace trog {

options_scene::options_scene(common_stuff &common_stuff) : 
        _common_stuff(common_stuff),
        _scroll(bn::regular_bg_items::hi_scores_bg.create_bg(8, 64))
         {

    _common_stuff.text_generator.set_center_alignment();
    _common_stuff.text_generator.set_palette_item(RED_PALETTE);
    _common_stuff.text_generator.generate(0, -72, "YE OLDE OPTIONS MENU", _header_sprites);

}

bn::optional<scene_type> options_scene::update(){
    bn::optional<scene_type> result;
    return result;
}

}