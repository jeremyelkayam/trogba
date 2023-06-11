#include "burninate_text.h"

#include "bn_sprite_items_trogdor_variable_8x16_font_black.h"
#include "bn_sprite_items_trogdor_variable_8x16_font_red.h"
#include "bn_sprite_items_cottagefire.h"

namespace trog{ 
    

burninate_text::burninate_text(bn::sprite_text_generator &generator) : 
    big_text(true, 0, 0, "BURNINATE!", 
    bn::sprite_items::trogdor_variable_8x16_font_red.palette_item(),
    bn::sprite_items::trogdor_variable_8x16_font_black.palette_item(), generator),
    _fire(bn::sprite_items::cottagefire.create_sprite(0, -10, 0)),
    _fire_move(_fire, 30, 0, -26) ,
    _fire_anim(bn::create_sprite_animate_action_forever(_fire, 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3))
    {
    _fire.set_scale(0.01);
    _fire.put_below();
    _fire_scale = bn::sprite_scale_loop_action(_fire, 30, 2);

}

void burninate_text::update(){
    _fire_move.update();
    _fire_scale->update();
    _fire_anim.update();
}

void burninate_text::set_visible(bool visible){
    big_text::set_visible(visible);
}

}