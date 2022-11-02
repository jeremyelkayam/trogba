#include <bn_core.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_timer.h>
// #include <bn_sprite_items_trogdor.h>
//debug settings for emulator
#define BN_LOG_BACKEND_MGBA true

int main()
{
    bn::core::init();
    bn::timer kicktimer;
    bool kicked=false;
    bn::sound_items::themesong.play(1);

    // bn::sprite_ptr sprite = bn::sprite_items::trogdor.create_sprite(0, 0);

    while(true)
    {
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
        bn::core::update();
    }
}
