#include <bn_core.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_timer.h>
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_regular_bg_items_titlebg.h"
#include "bn_sprite_items_trog.h"

//debug settings for emulator
#define BN_LOG_BACKEND_MGBA true

int main()
{
    bn::core::init();
    bn::timer kicktimer;
    bool kicked=false;
    bn::sound_items::themesong.play(1);

    bn::sprite_ptr mytrog = bn::sprite_items::trog.create_sprite(0, 0);
    bn::sprite_animate_action<4> action = bn::create_sprite_animate_action_forever(
                    mytrog, 5, bn::sprite_items::trog.tiles_item(), 0, 1, 2, 3);
    // bn::regular_bg_ptr regular_bg = bn::regular_bg_items::titlebg.create_bg(0, 0);

    while(true)
    {
        if(bn::keypad::left_held()){
            mytrog.set_horizontal_flip(true);
            mytrog.set_x(mytrog.x() - 1);
        }
        else if(bn::keypad::right_held()){
            mytrog.set_horizontal_flip(false);
            mytrog.set_x(mytrog.x() + 1);
        }

        if(bn::keypad::up_held()){
            mytrog.set_y(mytrog.y() - 1);
        }
        else if(bn::keypad::down_held()){
            mytrog.set_y(mytrog.y() + 1);
        }

        if(!kicked && bn::keypad::select_pressed()){
            BN_LOG("kick. TROGADOR");
            bn::sound_items::kick.play(1);
            kicktimer.restart();
            kicked=true;
        }
        if(kicked && kicktimer.elapsed_ticks() > 120000) { 
            bn::sound_items::trogador.play(1);
            kicked=false;
        }
        action.update();
        bn::core::update();
    }
}
