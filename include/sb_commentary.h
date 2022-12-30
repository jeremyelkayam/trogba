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
            static bool gameover(unsigned int score); // called by gameover_scene
            static void gameover_arch() 
                {bn::sound_items::sb_archer_gameover.play(_volume);}
            static void im_in_this_game()  // called by movie_scene
                {bn::sound_items::sb_iminthisgame.play(_volume);}
            static void i_sound_realistic()  // called by movie_scene
                {bn::sound_items::sb_isoundrealistic.play(_volume);}
            static void roast_kerrek() // called by movie_scene
                {bn::sound_items::sb_roastkerrek.play(_volume);}
            static void arrowed(); // called by play_scene
            static void sworded(); // called by play_scene
            static void burninate(); // called by play_scene
            static void level_win_pause(); // called by play_scene
            static void level_win_scene();
            static void stomp_peasant(); // called by peasant
            static void ignite_peasant(); // called by peasant
            static void ignite_cottage(); // called by cottage
    };
}