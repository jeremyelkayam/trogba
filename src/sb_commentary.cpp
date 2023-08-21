#include <bn_log.h>
#include "sb_commentary.h"

namespace trog { 

sb_commentary::sb_commentary(const bn::fixed &volume, bn::random &rand) : 
    _volume(volume), _rand(rand), _timer(0) {
}

bool sb_commentary::gameover(const unsigned int &score, const dragon &dragon){
    if(score >= TROG_GAMEOVER_SECRET_SCORE) {
        bn::sound_items::sb_secretthing.play(_volume);        
        return true;
    }else if(score >= TROG_GAMEOVER_BEAT_THAT_SCORE){
        bn::sound_items::sb_beatthatscore.play(_volume);
        return true;
    }else if(_rand.get_int(2) == 0){
        //todo: probably would be cleaner if we used a lookup table
        if(dragon == dragon::TROGDOR){
            bn::sound_items::sb_pineforyou.play(_volume);
        }else if(dragon == dragon::SUCKS){
            bn::sound_items::sb_itsover.play(_volume);
        }
        return true;
    }
    return false;
}

void sb_commentary::arrowed(const dragon &dragon){
    if(_rand.get_int(3) == 0 && _timer == 0){
        _timer = 1;

        if(dragon == dragon::TROGDOR){
            bn::sound_items::sb_stupid_archdeluxe.play(_volume);
        }else if(dragon == dragon::SUCKS){
            bn::sound_items::sb_arrowed.play(_volume);
        }

    }
}

//todo: maybe use inheritance for this? Idk there's def a better way 
void sb_commentary::sworded(const dragon &dragon){
    if(percent_chance(33.33333)){
        if(dragon == dragon::TROGDOR){
            play_random_sound({bn::sound_items::sb_stupidknight,
                    bn::sound_items::sb_worstgame});
        }else if(dragon == dragon::SUCKS){
            play_random_sound({bn::sound_items::sb_ihateu,
                    bn::sound_items::sb_sordid_affair});
        }
    }
}

void sb_commentary::burninate(const dragon &dragon){
    if(percent_chance(50)){
        if(dragon == dragon::TROGDOR){
            play_random_sound({bn::sound_items::sb_tablesturn,
                    bn::sound_items::sb_advantage});
        }else if(dragon == dragon::SUCKS){
            play_random_sound({bn::sound_items::sb_rampage});
        }

    }
}

void sb_commentary::level_win_pause(){
    if(percent_chance(20))
        play_random_sound({bn::sound_items::sb_ohyeah});
}

void sb_commentary::level_win_scene(){
    if(percent_chance(20))
        play_random_sound({bn::sound_items::sb_comesinthenight,
                bn::sound_items::sb_bestgame});
}

void sb_commentary::stomp_peasant(){
    if(percent_chance(5))
        play_random_sound({bn::sound_items::sb_asquisha, 
                bn::sound_items::sb_asquishasquisha,
                bn::sound_items::sb_squishedyou});
}

void sb_commentary::ignite_peasant(){
    if(percent_chance(20))
        play_random_sound({bn::sound_items::sb_hesonfire,
                bn::sound_items::sb_lookit_him_burning});
}

void sb_commentary::ignite_cottage(){
    if(percent_chance(20))
        play_random_sound({bn::sound_items::sb_dooj});
}

void sb_commentary::update(){
    if(_timer > 0){
        --_timer;
    }
}

void sb_commentary::play_random_sound(std::initializer_list<bn::sound_item> sounds){    
    if(_timer == 0){
        uint8_t rand_num = _rand.get_int(sounds.size());
        BN_LOG("random nunber", rand_num);   
        uint8_t counter = 0;
        for(bn::sound_item sound : sounds){
            if(counter == rand_num){
                sound.play(_volume);
                break;
            }
            ++counter;
        }
        _timer = 255;
    }
}


bool sb_commentary::percent_chance(const bn::fixed &pct){
    BN_ASSERT(pct >= 0, "percent must be between 0 and 100");
    BN_ASSERT(pct < 100, "percent must be between 0 and 100");
    return pct > rand.get_fixed(100);
} 

}

