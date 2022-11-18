#include <bn_keypad.h>
#include <bn_log.h>
#include "scene.h"
namespace trog{

//Dev tool to position backgrounds.    
void scene::move_background(bn::regular_bg_ptr bg) {
    if(bn::keypad::up_held()){
        bg.set_y(bg.y() - 1);
        BN_LOG("ycor ", bg.y());
    }
    if(bn::keypad::down_held()){
        bg.set_y(bg.y() + 1);
        BN_LOG("ycor ", bg.y());
    }
    if(bn::keypad::left_held()){
        bg.set_x(bg.x() - 1);
        BN_LOG("xcor ", bg.x());
    }
    if(bn::keypad::right_held()){
        bg.set_x(bg.x() + 1);
        BN_LOG("xcor ", bg.x());
    }
}

} // namespace trog