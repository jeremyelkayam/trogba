#include <bn_keypad.h>
#include "Player.h"

Player::Player(){

}

void Player::update(){
    if(bn::keypad::left_held()){
            sprite.set_horizontal_flip(true);
            sprite.set_x(sprite.x() - 1);
            // this sucks need to change 
            walkcycle.set_wait_updates(5);
        }
        else if(bn::keypad::right_held()){
            sprite.set_horizontal_flip(false);
            sprite.set_x(sprite.x() + 1);
            walkcycle.set_wait_updates(5);
        }

        if(bn::keypad::up_held()){
            sprite.set_y(sprite.y() - 1);
            walkcycle.set_wait_updates(5);
        }
        else if(bn::keypad::down_held()){
            sprite.set_y(sprite.y() + 1);
            walkcycle.set_wait_updates(5);
        }
        if(!bn::keypad::down_held() && !bn::keypad::up_held() && !bn::keypad::right_held() && !bn::keypad::right_held()){
            //if nothing is held down, we
            walkcycle.set_wait_updates(50000);
        }
        walkcycle.update();
}