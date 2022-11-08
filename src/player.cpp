#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_log.h>
#include <bn_sound_items.h>
#include "player.h"
#include "entity.h"
namespace trog {

player::player() : 
        entity(bn::fixed(0), bn::fixed(0), bn::fixed(20), bn::fixed(45), bn::sprite_items::player.create_sprite(0, 0)),
        speed(bn::fixed(1.5)),
        walkcycle(bn::create_sprite_animate_action_forever(
                    _sprite, 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3)){
    _sprite.set_z_order(FRONT_ZORDER);

    trogmeter = 0;
    burninate_time = 0;
}

bool player::burninating(){
    return burninate_time != 0;
}

void player::update(){
    entity::update();
    move();
    check_boundary_collision();


    _sprite.set_position(_pos);


    if(burninate_time > 0) { 
        burninate_time--;
    }else{
        breath.disable();
    }
    //update fire breath;
    short xoffset = TROG_FIREBREATH_XOFFSET;
    if(_sprite.horizontal_flip()){
        xoffset=-xoffset;
    }
    breath.set_x(_pos.x() + xoffset);
    breath.set_y(_pos.y() + TROG_FIREBREATH_YOFFSET);
    breath.update();

}

void player::move(){

    bool moving = false;
    
    if(bn::keypad::up_held()){
        _pos.set_y(_pos.y() - speed);
        moving=true;
    }
    if(bn::keypad::down_held()){
        _pos.set_y(_pos.y() + speed);
        moving=true;
    }
    if(bn::keypad::left_held()){
        _sprite.set_horizontal_flip(true);
        breath.set_horizontal_flip(true);
        _pos.set_x(_pos.x() - speed);
        moving=true;
    }
    if(bn::keypad::right_held()){
        _sprite.set_horizontal_flip(false);
        breath.set_horizontal_flip(false);
        _pos.set_x(_pos.x() + speed);
        moving=true;
    }
    if(moving){
        walkcycle.update();
    }

}

void player::check_boundary_collision(){
    bn::fixed top_bound = -70;
    bn::fixed bottom_bound = 81;
    bn::fixed left_bound = -120;
    bn::fixed right_bound = 120;
    //collision
    if(_hitbox.top() < top_bound){
        _pos.set_y(top_bound + _hitbox.height() / 2);
    }
    if(_hitbox.bottom() > bottom_bound){
        _pos.set_y(bottom_bound - _hitbox.height() / 2);
    }
    if(_hitbox.left() < left_bound){
        _pos.set_x(left_bound + _hitbox.width() / 2);
    }
    if(_hitbox.right() > right_bound){
        _pos.set_x(right_bound - _hitbox.width() / 2);
    }
    
    _hitbox.set_position(_pos);
    
}

void player::check_cottage_collision(cottage &cottage){
    bn::fixed_rect cottagebox = cottage.get_hitbox();
    if(_hitbox.intersects(cottagebox)){
        // BN_LOG("collision lol make him stop");
    }
}

void player::check_peasant_collision(peasant &peasant){
    bn::fixed_rect pbox = peasant.get_hitbox();
    if(_hitbox.intersects(pbox) && !peasant.dead()){
        BN_LOG("stomped.");
        peasant.stomp();
        ++trogmeter;
        if(trogmeter == trogmeter_max){
            //burnination logic
            burninate_time = burninate_length;
            breath.enable();

            BN_LOG("aaron burrninate. got milk");
            bn::sound_items::burninate.play(1);
        }
    }
}


}
