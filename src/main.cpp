#include <bn_core.h>
#include <bn_log.h>
#include <bn_keypad.h>
#include <bn_sound_items.h>
#include <bn_timer.h>
#include "bn_regular_bg_items_titlebg.h"
#include "Player.h"

//debug settings for emulator
#define BN_LOG_BACKEND_MGBA true

int main()
{
    bn::core::init();
    bn::timer kicktimer;
    bool kicked=false;
    bn::sound_items::themesong.play(1);
    Player tragdar = Player();

    // bn::regular_bg_ptr regular_bg = bn::regular_bg_items::titlebg.create_bg(0, 0);

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
        tragdar.update();
        bn::core::update();
    }
}
