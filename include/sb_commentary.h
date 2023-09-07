// sb_commentary.h
// Purpose: Static functions for calling Strong Bad's commentary

#pragma once
#include <bn_random.h>
#include <bn_vector.h>
#include <bn_string.h>
#include <bn_unordered_map.h>

#include "enums.h"
#include "constants.h"
#include "voice_clips.h"

namespace trog {

    class sb_commentary { 
        protected:
            const bn::fixed &_volume;
            bn::random &_rand;
            uint8_t _timer;
            const voice_clips &get_clips(const dragon &dragon) ;
            void play_random_sound(const bn::sound_item *item, const uint8_t &count);
            void play_sound(const bn::sound_item &item);



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
            void level_win_pause(const dragon &dragon, const bool &troghammer_arrived); // called by play_scene
            void level_win_scene(const dragon &dragon); // called by level_win_scene
            void stomp_peasant(const dragon &dragon); // called by peasant
            void ignite_peasant(const dragon &dragon); // called by peasant
            void ignite_cottage(const dragon &dragon); // called by cottage
            bool select_character(const dragon &dragon);


            void update();

            bool percent_chance(const bn::fixed &pct);
    };
}