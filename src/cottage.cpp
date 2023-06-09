
#include "cottage.h"

#include "bn_sprite_items_cottage.h"
#include "bn_sprite_items_cottage_burninated.h"
#include "bn_sprite_items_cottage_bits.h"
#include "bn_sprite_items_cottagefire.h"

namespace trog {

cottage::cottage(bn::fixed xcor, bn::fixed ycor, direction direction) : 
        entity(xcor, ycor, 20, 20, bn::sprite_items::cottage.create_sprite(xcor, ycor + 1, 2)),
        _direction(direction){

    bn::sprite_ptr tippytop = bn::sprite_items::cottage_bits.create_sprite(xcor + 1, ycor - 19, 2);
    tippytop.put_below();
    switch(direction) {
        case direction::UP:
            _sprite.set_tiles(bn::sprite_items::cottage.tiles_item(), 0);
            tippytop.set_tiles(bn::sprite_items::cottage_bits.tiles_item(), 0);
            tippytop.set_x(xcor + 3);
        break;
        case direction::DOWN:
            _sprite.set_tiles(bn::sprite_items::cottage.tiles_item(), 1);
            tippytop.set_tiles(bn::sprite_items::cottage_bits.tiles_item(), 1);
            tippytop.set_x(xcor + 3);
        break;
        case direction::RIGHT:
            _sprite.set_horizontal_flip(true);
            tippytop.set_horizontal_flip(true);
            tippytop.set_x(xcor - 1);
        break;
        default:
        break;
    }
    _time_burning = 0;

    _sprite.set_z_order(BACK_ZORDER);
    tippytop.set_z_order(BACK_ZORDER);
    
    _bits.emplace_back(tippytop);

}

void cottage::update(){
    if(_flames){
        _burningflames->update();
        ++_time_burning;
        if(_time_burning > 120){
            //clear out our flames; they are no longer necessary.
            _flames->set_visible(false);
            _burningflames.reset();
            _flames.reset();
        }   
    }

    if(_time_burning == 61){
        bn::sprite_ptr smaller_bit = bn::sprite_items::cottage_bits.create_sprite(_pos.x(), _pos.y(), 3);
        bn::sprite_ptr bigger_bit = bn::sprite_items::cottage_bits.create_sprite(_pos.x(), _pos.y(), 4);
        smaller_bit.set_z_order(_sprite.z_order());
        bigger_bit.set_z_order(_sprite.z_order());

        bn::fixed yoffset = 13, xoffset = 20;

        if(_direction == direction::UP || _direction == direction::DOWN){
            // bigger_bit.set_horizontal_flip(true);
            smaller_bit.set_x(_pos.x() - xoffset);
            smaller_bit.set_y(_pos.y() + yoffset);
            bigger_bit.set_x(_pos.x() + xoffset);
            bigger_bit.set_y(_pos.y() + yoffset);
        }else{
            if(_direction == direction::LEFT){
                bigger_bit.set_horizontal_flip(true);
                smaller_bit.set_horizontal_flip(true);
                xoffset = -xoffset;
            }
            smaller_bit.set_x(_pos.x() - xoffset);
            smaller_bit.set_y(_pos.y() + yoffset);
            bigger_bit.set_x(_pos.x() + xoffset);
            bigger_bit.set_y(_pos.y() + yoffset);
            
        }

        //default can be the sideways one because it corresponds to 2 cases
        int newindex = 2;
        if(_direction == direction::UP){
            newindex = 0;
        }else if(_direction == direction::DOWN){
            newindex = 1;
        }
        _sprite.set_item(bn::sprite_items::cottage_burninated, newindex);
        _sprite.set_y(_pos.y() + 1);
        _bits.clear();        
        _bits.emplace_back(smaller_bit);
        _bits.emplace_back(bigger_bit);
    }
}

bool cottage::burninate(){
    if(_time_burning==0){
        _flames = bn::sprite_items::cottagefire.create_sprite_optional(_pos.x(), _pos.y() + -10);
        _burningflames = bn::create_sprite_animate_action_forever(
                    _flames.value(), 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3);

        _time_burning=1;
        _flames->set_z_order(MID_ZORDER);
        return true;
    }
    return false;
}
}