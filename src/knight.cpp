#include <bn_log.h>
#include "knight.h"

namespace trog { 

knight::knight(bn::fixed xcor, bn::fixed ycor, bool facingRight, bn::random &random, bn::fixed speed, uint8_t cycletime) :
    freezable(xcor, ycor, bn::fixed(TROG_KNIGHT_WIDTH), bn::fixed(TROG_KNIGHT_HEIGHT), bn::sprite_items::knight.create_sprite(xcor, ycor)),
    _walkcycle_timer(0),
    _rotation(0),
    _speed(speed),
    _cycletime(cycletime),
    _random(random),
    //todo: this is incomplete; fix it plz
    _walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 20, bn::sprite_items::knight.tiles_item(), 0, 3, 2, 3))
{
    _sprite.set_horizontal_flip(facingRight);
}

// knight::knight(bn::fixed xcor, bn::fixed ycor, bool facingRight, bn::random &random) {
// //  
// }

void knight::update_anim(){
    entity::update_anim();
    if(_move_action || _move_by_action || _update_anim_when_not_moving){
        _walkcycle.update();
    }
}

void knight::update(){
    entity::update();

    if(!_freeze_timer){
        ++_walkcycle_timer;
        _walkcycle.update();

        bn::fixed xdist = 0;
        bn::fixed ydist = 0;

        if(alerted()){
            update_alert();
            xdist = _alert_dist->x();
            ydist = _alert_dist->y();
            if(_walkcycle_timer == _cycletime / 2) {
                _walkcycle_timer = 0;
                _alert_icon.set_visible(false);
                _alert_dist.reset();
            }
        }else{
            if(_walkcycle_timer == _cycletime) {
                _walkcycle_timer = 0;
            }
            //todo i hate these 'while true' / 'break' loops
            // change this one
            while(true){
                if(_walkcycle_timer < _cycletime / 2){
                    xdist = _speed;
                    ydist = -_speed;
                }else{
                    xdist = -(_speed - bn::fixed(0.1));
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

        }

        _pos.set_x(_pos.x() + xdist);
        _pos.set_y(_pos.y() + ydist);
    }else update_freeze();
}

void knight::change_direction(){
    _rotation = _random.get_int(0,4);
}

void knight::alert(bn::fixed_point stomp_pos){
    freezable::alert(stomp_pos);
    _walkcycle_timer = 0;
    update_alert_direction(stomp_pos);
}

void knight::update_alert_direction(bn::fixed_point pos){
    bn::fixed xdist = (pos.x() - _pos.x());
    bn::fixed ydist = (pos.y() - _pos.y());
    bn::fixed angle = bn::atan2(ydist.round_integer(), xdist.round_integer());

    _alert_dist.emplace(cos(angle) * _speed, sin(angle) * _speed);
}

}