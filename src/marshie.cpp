#include "marshie.h"

namespace trog { 

marshie::marshie(int xcor, int ycor) : 
        entity(xcor, ycor, 0, 0, bn::sprite_items::marshie.create_sprite(xcor, ycor)),
        _anim(bn::create_sprite_animate_action_forever(
                _sprite, 30, bn::sprite_items::marshie.tiles_item(),
                1, 2, 3, 0)) {
    _sprite.set_z_order(MID_ZORDER);
    update_anim();
}

void marshie::update(){
    entity::update();
}

void marshie::update_anim(){
    entity::update_anim();
    _anim.update();

    if(_pos.y() > 0)
    {
        _pos.set_y(_pos.y() - 4);
    }


    // BERNOULLI LEMNISCATE AROUND THE CENTER POINT
    if(_timer == FIG8_TIME)
    {
        _timer -= FIG8_TIME;
    }
    
    bn::fixed t = (bn::fixed(_timer) / FIG8_TIME);

    bn::fixed A = 50;

    bn::pair<bn::fixed, bn::fixed> sin_cos_t = bn::sin_and_cos(t);
    bn::fixed& sint = sin_cos_t.first;
    bn::fixed& cost = sin_cos_t.second;

    bn::fixed denom = 1 + sint * sint;
    bn::fixed x = A * cost / denom;
    bn::fixed y = A * cost * sint / denom;

    _sprite.set_x(_pos.x() + x);
    _sprite.set_y(_pos.y() + y);
    ++_timer;
}

}

