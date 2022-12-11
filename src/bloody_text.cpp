#include <bn_log.h>

#include "bloody_text.h"
#include "bn_sprite_items_blood_droplet_short_16px.h"
#include "bn_sprite_items_blood_droplet_long_16px.h"
#include "bn_sprite_items_blood_droplet_short_24px.h"
#include "bn_sprite_items_blood_droplet_long_24px.h"
// #include "bn_sprite_items_troghammer.h"

#define LONG_DROPLET (_bigger ? bn::sprite_items::blood_droplet_long_24px : bn::sprite_items::blood_droplet_long_16px)
#define SHORT_DROPLET (_bigger ? bn::sprite_items::blood_droplet_short_24px : bn::sprite_items::blood_droplet_short_16px)
namespace trog { 



bloody_text::bloody_text(bool bigger, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette) : 
    big_text(bigger, x, y, text, palette) {
    

    _droplets.emplace_front(LONG_DROPLET, x.integer(), y.integer(), 0);
}


void bloody_text::set_visible(bool visible){
    big_text::set_visible(visible);
}

void bloody_text::update(){
    for(auto &d : _droplets) {
        d.update();
    }
}

blood_droplet::blood_droplet(bn::sprite_item item, int xcor, int ycor, int anim_index) : 
    _droplet(bn::sprite_items::blood_droplet_long_24px.create_sprite(xcor, ycor)),
    _anim(bn::create_sprite_animate_action_forever(
            _droplet, 5, bn::sprite_items::blood_droplet_long_24px.tiles_item(), anim_index % 5,anim_index % 5 + 1, anim_index % 5 + 2, anim_index % 5 + 3, anim_index % 5 + 4)) {

}

void blood_droplet::update(){
    BN_LOG("AYY");
    _anim.update();
}

}
