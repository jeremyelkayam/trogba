#include <bn_sound_items.h>
#include <bn_log.h>

#include "cottage.h"
#include "sb_commentary.h"

#include "bn_sprite_items_cottage.h"
#include "bn_sprite_items_cottage_burninated.h"
#include "bn_sprite_items_cottagefire.h"

namespace trog {

cottage::cottage(bn::fixed xcor, bn::fixed ycor, direction direction, bool has_treasure, bool burninated, common_stuff &common_stuff) : 
        entity(xcor, ycor, TROG_COTTAGE_HITBOX_WIDTH, TROG_COTTAGE_HITBOX_HEIGHT, bn::sprite_items::cottage.create_sprite(xcor, ycor, 2)),
        _direction(direction),
        _has_treasure(has_treasure),
        _common_stuff(common_stuff) {
    switch(direction) {
        case direction::UP:
            _sprite.set_tiles(bn::sprite_items::cottage.tiles_item(), 0);
        break;
        case direction::DOWN:
            _sprite.set_tiles(bn::sprite_items::cottage.tiles_item(), 1);
        break;
        case direction::RIGHT:
            _sprite.set_horizontal_flip(true);
        break;
        default:
        break;
    }
    _time_burning = 0;

    _sprite.set_z_order(BACK_ZORDER);

    if(burninated) _time_burning = TROG_COTTAGEFIRE_TIME + 1;

}

void cottage::update(){

    if(_flames){
        _burningflames->update();
        ++_time_burning;
        if(_time_burning > TROG_COTTAGEFIRE_TIME){
            //clear out our flames; they are no longer necessary.
            _flames->set_visible(false);
            _burningflames.reset();
            _flames.reset();
        }   
    }
    
    if(burninated()){
        //default can be the sideways one because it corresponds to 2 cases
        int newindex = 2;
        if(_direction == direction::UP){
            newindex = 0;
        }else if(_direction == direction::DOWN){
            newindex = 1;
        }
        _sprite.set_item(bn::sprite_items::cottage_burninated, newindex);
        _sprite.set_y(_pos.y() + 2);
    }
}

bool cottage::burninate(){
    if(_time_burning==0){
        _flames = bn::sprite_items::cottagefire.create_sprite_optional(_pos.x(), _pos.y() + TROG_COTTAGEFIRE_YOFFSET);
        _burningflames = bn::create_sprite_animate_action_forever(
                    _flames.value(), 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3);
        _time_burning=1;
        _flames->set_z_order(MID_ZORDER);
        bn::sound_items::burningcottage.play(_common_stuff.savefile.options.sound_vol);
        return true;
    }
    return false;
}

bool cottage::has_treasure(){
    return _has_treasure && _time_burning==0;
}

void cottage::set_visible(const bool &visible){
    entity::set_visible(visible);
    if(_flames) { //if the cottage is on fire
        _flames->set_visible(visible);
    }
}

}