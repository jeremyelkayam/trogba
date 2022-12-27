// sb_commentary.h
// Purpose: Static functions for calling Strong Bad's commentary

#pragma once
#include <bn_sound_items.h>
#include <bn_random.h>

#include "constants.h"

namespace trog {
    class sb_commentary { 
        protected:
            constexpr static bn::fixed _volume=1;
            
        public:
            sb_commentary();
            static bool gameover(unsigned int score);
            static void im_in_this_game()  {bn::sound_items::sb_iminthisgame.play(_volume);}
            static void i_sound_realistic()  {bn::sound_items::sb_isoundrealistic.play(_volume);}
            static void roast_kerrek() {bn::sound_items::sb_roastkerrek.play(_volume);}

    };
}