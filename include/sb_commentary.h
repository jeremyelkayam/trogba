// sb_commentary.h
// Purpose: Static functions for calling Strong Bad's commentary

#pragma once
#include <bn_sound_items.h>
#include <bn_random.h>
#include <bn_vector.h>
#include <initializer_list>

#include "enums.h"
#include "constants.h"

namespace trog {
    class sb_commentary { 
        protected:
            const bn::fixed &_volume;
            bn::random &_rand;
            uint8_t _timer;
            void play_random_sound(std::initializer_list<bn::sound_item> item);

        public:
            sb_commentary(const bn::fixed &volume, bn::random &rand);
            bool gameover(const unsigned int &score, const dragon &dragon); // called by gameover_scene
            void gameover_arch()  // called by movie_scene
                {bn::sound_items::sb_archer_gameover.play(_volume);}
            void im_in_this_game()  // called by movie_scene
                {bn::sound_items::sb_iminthisgame.play(_volume);}
            void i_sound_realistic()  // called by movie_scene
                {bn::sound_items::sb_isoundrealistic.play(_volume);}
            void roast_kerrek() // called by movie_scene
                {bn::sound_items::sb_roastkerrek.play(_volume);}
            void arrowed(const dragon &dragon); // called by play_scene
            void sworded(const dragon &dragon); // called by play_scene
            void burninate(const dragon &dragon); // called by play_scene
            void level_win_pause(); // called by play_scene
            void level_win_scene(); // called by level_win_scene
            void stomp_peasant(); // called by peasant
            void ignite_peasant(); // called by peasant
            void ignite_cottage(); // called by cottage

            void update();

            bool percent_chance(const bn::fixed &pct);
    };
}