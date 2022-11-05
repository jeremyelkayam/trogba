#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_log.h>
#include "player.h"
namespace trog {

player::player() : 
        pos(bn::fixed(0), bn::fixed(0)),
        hitbox(bn::fixed(0), bn::fixed(0), bn::fixed(20), bn::fixed(45)),
        speed(bn::fixed(1.5)),
        direction(bn::fixed(0)),
        sprite(bn::sprite_items::player.create_sprite(0, 0)),
        walkcycle(bn::create_sprite_animate_action_forever(
                    sprite, 5, bn::sprite_items::player.tiles_item(), 0, 1, 2, 3)){
    trogmeter = 0;
    burninate_time = 0;
}

bool player::burninating(){
    return burninate_time != 0;
}

void player::update(){
    move();
    check_collision();


    sprite.set_position(pos);

    //burnination logic
    if(trogmeter == trogmeter_max) {
        burninate_time = burninate_length;
    }
    if(burninate_time > 0) { 
        burninate_time--;
    }

}

void player::move(){

    bool moving = false;
    
    if(bn::keypad::up_held()){
        pos.set_y(pos.y() - speed);
        moving=true;
    }
    if(bn::keypad::down_held()){
        pos.set_y(pos.y() + speed);
        moving=true;
    }
    if(bn::keypad::left_held()){
        sprite.set_horizontal_flip(true);
        pos.set_x(pos.x() - speed);
        moving=true;
    }
    if(bn::keypad::right_held()){
        sprite.set_horizontal_flip(false);
        pos.set_x(pos.x() + speed);
        moving=true;
    }
    if(moving){
        walkcycle.update();
    }

    hitbox.set_position(pos);
}
void player::check_collision(){
    bn::fixed top_bound = -70;
    bn::fixed bottom_bound = 80;
    bn::fixed left_bound = -120;
    bn::fixed right_bound = 120;
    //collision
    if(hitbox.top() < top_bound){
        pos.set_y(top_bound + hitbox.height() / 2);
    }
    if(hitbox.bottom() > bottom_bound){
        pos.set_y(bottom_bound - hitbox.height() / 2);
    }
    if(hitbox.left() < left_bound){
        pos.set_x(left_bound + hitbox.width() / 2);
    }
    if(hitbox.right() > right_bound){
        pos.set_x(right_bound - hitbox.width() / 2);
    }
    
    hitbox.set_position(pos);
    
    // BN_LOG("hitbox bottom", hitbox.bottom());
    // BN_LOG("hitbox left", hitbox.left());
    // BN_LOG("hitbox right", hitbox.right());
}




}
