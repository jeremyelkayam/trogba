#include <bn_sound_items.h>
#include "cottage.h"

#include "bn_sprite_items_cottagedown.h"
#include "bn_sprite_items_cottageup.h"
#include "bn_sprite_items_cottageleft.h"
#include "bn_sprite_items_cottagefire.h"
#include "bn_sprite_items_cottagedown_burninated.h"
#include "bn_sprite_items_cottageup_burninated.h"
#include "bn_sprite_items_cottageleft_burninated.h"

namespace trog {

cottage::cottage(bn::fixed xcor, bn::fixed ycor, direction direction, bool has_treasure) : 
        entity(xcor, ycor, TROG_COTTAGE_HITBOX_WIDTH, TROG_COTTAGE_HITBOX_HEIGHT, bn::sprite_items::cottageleft.create_sprite(xcor, ycor)),
        _direction(direction),
        _has_treasure(has_treasure),
        _flames(bn::sprite_items::cottagefire.create_sprite(xcor, ycor + TROG_COTTAGEFIRE_YOFFSET)),
        _burningflames(bn::create_sprite_animate_action_forever(
                    _flames, 10, bn::sprite_items::cottagefire.tiles_item(), 0, 1, 2, 3)) {
    switch(direction) {
        case direction::UP:
            _sprite.set_item(bn::sprite_items::cottageup);
        break;
        case direction::DOWN:
            _sprite.set_item(bn::sprite_items::cottagedown);
        break;
        case direction::RIGHT:
            _sprite.set_horizontal_flip(true);
        break;
        default:
        break;
    }
    _time_burning = 0;

    _flames.set_visible(false);
    _sprite.set_z_order(BACK_ZORDER);

}

void cottage::update(){
    if(_flames.visible()){
        _burningflames.update();
        ++_time_burning;
    }
    if(_time_burning > TROG_COTTAGEFIRE_TIME){
        _flames.set_visible(false);
    }
    if(burninated()){
        //default can be the sideways one because it corresponds to 2 cases
        bn::sprite_item newsprite = bn::sprite_items::cottageleft_burninated;
        if(_direction == direction::UP){
            newsprite = bn::sprite_items::cottageup_burninated;
        }else if(_direction == direction::DOWN){
            newsprite = bn::sprite_items::cottagedown_burninated;
        }
        _sprite.set_item(newsprite);
    }
}

bool cottage::burninate(){
    if(_time_burning==0){
        _flames.set_visible(true);
        _time_burning=1;
        bn::sound_items::burningcottage.play(TROG_DEFAULT_VOLUME);
        return true;
    }
    return false;
}

bool cottage::has_treasure(){
    return _has_treasure && _time_burning==0;
}

void cottage::set_visible(bool visible){
    entity::set_visible(visible);
    if(_time_burning > 0) { //if the cottage is on fire
        _flames.set_visible(visible);
    }
}

}