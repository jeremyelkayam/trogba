#include <bn_log.h>
#include "knight.h"

namespace trog { 

knight::knight(int xcor, int ycor, bool facingRight) :
    entity(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
    _timer(0),
    _rotation(0),
    //todo: this is incomplete; fix it plz
    _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 20, bn::sprite_items::knight.tiles_item(), 0, 3, 2, 3))
{
    _sprite.set_horizontal_flip(facingRight);
}

void knight::update(){
    entity::update();
    _walkcycle.update();
    ++_timer;
    bn::fixed speed = 0.35;


    if(_timer == 220) {
        _timer = 0;
    }

    bn::fixed xdist = 0;
    bn::fixed ydist = 0;

    //todo i hate these 'while true' / 'break' loops
    // change this one
    while(true){
        if(_timer < 110){
            xdist = speed;
            ydist = -speed;
        }else{
            xdist = -(speed - 0.05);
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