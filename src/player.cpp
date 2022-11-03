#include <bn_keypad.h>
#include "player.h"
namespace trog {

player::player(){
    trogmeter = 0;
    burninate_time = 0;
}

bool player::burninating(){
    return burninate_time != 0;
}

void player::update(){

    //burnination logic
    if(trogmeter == trogmeter_max) {
        burninate_time = burninate_length;
    }
    if(burninate_time > 0) { 
        burninate_time--;
    }

    if(bn::keypad::left_held()){
            sprite.set_horizontal_flip(true);
            sprite.set_x(sprite.x() - 2);
            // this sucks need to change 
        }
        else if(bn::keypad::right_held()){
            sprite.set_horizontal_flip(false);
            sprite.set_x(sprite.x() + 2);
        }

        if(bn::keypad::up_held()){
            sprite.set_y(sprite.y() - 2);
        }
        else if(bn::keypad::down_held()){
            sprite.set_y(sprite.y() + 2);
        }
        if(!bn::keypad::down_held() && !bn::keypad::up_held() && !bn::keypad::left_held() && !bn::keypad::right_held()){
            //if nothing is held down, we pause the walk cycle by setting the wait updates WAY FUCKING HIGH 
            walkcycle.set_wait_updates(50000);
        }else{
            walkcycle.set_wait_updates(3);
        }
        walkcycle.update();
}

}
