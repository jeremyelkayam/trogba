#include "knight.h"

namespace trog { 

knight::knight(bn::fixed xcor, bn::fixed ycor, bool facingRight, bn::random &random) :
    entity(xcor, ycor, bn::fixed(9), bn::fixed(18), bn::sprite_items::knight.create_sprite(xcor, ycor)),
    _timer(0),
    _rotation(0),
    _speed(0.522),
    _cycletime(180),
    _random(random),
    //todo: this is incomplete; fix it plz
    _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 20, bn::sprite_items::knight.tiles_item(), 0, 1))
{
    _sprite.set_horizontal_flip(facingRight);
}

void knight::update(){
    entity::update();
    _walkcycle.update();
    ++_timer;


    if(_timer == _cycletime) {
        _timer = 0;
    }

    bn::fixed xdist = 0;
    bn::fixed ydist = 0;

    //todo i hate these 'while true' / 'break' loops
    // change this one
    while(true){
        if(_timer < _cycletime / 2){
            xdist = _speed;
            ydist = -_speed;
        }else{
            xdist = -(_speed - 0.1);
        }

        if(_sprite.horizontal_flip()){
            xdist = -xdist;
        }

        switch(_rotation){
            case 1:
                ydist = -ydist;
                bn::swap(xdist, ydist);
                break;
            case 2:
                xdist = -xdist;
                ydist = -ydist;
                break;
            case 3:
                xdist = -xdist;
                bn::swap(xdist, ydist);                
                break;
            default:
                break;
            }
        

        if(going_to_go_offscreen_x(_pos.x() + xdist) || 
            going_to_go_offscreen_y(_pos.y() + ydist) ){
            change_direction();
        }else{
            break;
        }
    }

    _pos.set_x(_pos.x() + xdist);
    _pos.set_y(_pos.y() + ydist);

}

void knight::change_direction(){
    _rotation = _random.get_int(0,4);
}

}