#include "bloody_text.h"
#include "bn_sprite_items_blood_droplet_short_16px.h"
#include "bn_sprite_items_blood_droplet_long_16px.h"
#include "bn_sprite_items_blood_droplet_short_24px.h"
#include "bn_sprite_items_blood_droplet_long_24px.h"
// #include "bn_sprite_items_troghammer.h"

#define LONG_DROPLET (_bigger ? bn::sprite_items::blood_droplet_long_24px : bn::sprite_items::blood_droplet_long_16px)
#define SHORT_DROPLET (_bigger ? bn::sprite_items::blood_droplet_short_24px : bn::sprite_items::blood_droplet_short_16px)
namespace trog { 



bloody_text::bloody_text(bool bigger, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette, bn::random &rand) : 
    big_text(bigger, x, y, text, palette) {
    int ycor;
    if(_bigger){
        ycor = y.integer() + 27;
    }else{
        ycor = y.integer() + 16;
    }
    for(int z = 0 ; z < _text_sprites.size() - 1 ; ++z){
        int xcor = _text_sprites.at(z).x().integer();
        bool short_droplet = rand.get_int(2);
        _droplets.emplace_back(short_droplet ? SHORT_DROPLET : LONG_DROPLET, xcor + rand.get_int(8), ycor, rand.get_int(5));
    }
}


void bloody_text::set_visible(bool visible){
    big_text::set_visible(visible);
}

void bloody_text::update(){
    for(auto &d : _droplets) {
        d.update();
    }
}

void bloody_text::set_blending_enabled(bool enabled){
    for(blood_droplet &drop : _droplets){
        drop.set_blending_enabled(enabled);
    }
    for(bn::sprite_ptr &sprite : _text_sprites){
        sprite.set_blending_enabled(enabled);
    }
}

blood_droplet::blood_droplet(bn::sprite_item item, int xcor, int ycor, int anim_index) : 
    _droplet(item.create_sprite(xcor, ycor)),
    _anim(bn::create_sprite_animate_action_forever(
            _droplet, 5, item.tiles_item(), anim_index % 5,(anim_index + 1) % 5 , (anim_index + 2) % 5, (anim_index + 3) % 5, (anim_index + 4) % 5)) {

}

void blood_droplet::update(){
    _anim.update();
}

}
