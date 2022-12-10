#include "bloody_text.h"
#include "bn_sprite_items_blood_droplet_short_16px.h"
#include "bn_sprite_items_blood_droplet_long_16px.h"
#include "bn_sprite_items_blood_droplet_short_24px.h"
#include "bn_sprite_items_blood_droplet_long_24px.h"
namespace trog { 



bloody_text::bloody_text(bool bigger, const bn::fixed &x, const bn::fixed &y, const char *text, const bn::sprite_palette_item &palette) : 
    big_text(bigger, x, y, text, palette){
    // _blood_droplets.emplace_back()
}

void bloody_text::set_visible(bool visible){
    big_text::set_visible(visible);
}

void bloody_text::update(){
    //todo: animate the blood
}

}