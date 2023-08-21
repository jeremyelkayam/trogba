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

void sb_commentary::sworded(const dragon &dragon){
    if(_timer == 0){
        unsigned short rand_num = _rand.get_int(5);
        BN_LOG("number: ", rand_num);
        if(rand_num == 0){

            if(dragon == dragon::TROGDOR){
                bn::sound_items::sb_stupidknight.play(_volume);
            }else if(dragon == dragon::SUCKS){
                bn::sound_items::sb_sordid_affair.play(_volume);
                BN_LOG("a sordid affair");
            }
            _timer = 1;
        }else if(rand_num == 1){
            if(dragon == dragon::TROGDOR){
                bn::sound_items::sb_worstgame.play(_volume);
            }else if(dragon == dragon::SUCKS){
                bn::sound_items::sb_ihateu.play(_volume);
            }

            _timer = 1;
        }
    }
}

void sb_commentary::burninate(const dragon &dragon){
    if(_timer == 0){
        unsigned short rand_num = _rand.get_int(4);
        if(rand_num == 0){
            bn::sound_items::sb_tablesturn.play(_volume);
            _timer = 1;
        }else if(rand_num == 1){
            bn::sound_items::sb_advantage.play(_volume);
            _timer = 1;
        }
    }
}

void sb_commentary::level_win_pause(){
    unsigned short rand_num = _rand.get_int(5);
    if(_timer == 0 && rand_num == 0){
        bn::sound_items::sb_ohyeah.play(_volume);
        _timer = 1;

    }
}
void sb_commentary::level_win_scene(){
    unsigned short rand_num = _rand.get_int(10);

    play_random_sound({bn::sound_items::sb_comesinthenight,
            bn::sound_items::sb_bestgame});
}

void sb_commentary::stomp_peasant(){
    play_random_sound({bn::sound_items::sb_asquisha, 
            bn::sound_items::sb_asquishasquisha,
            bn::sound_items::sb_squishedyou});
}

void sb_commentary::ignite_peasant(){
    play_random_sound({bn::sound_items::sb_hesonfire,
            bn::sound_items::sb_lookit_him_burning});
}

void sb_commentary::ignite_cottage(){
    unsigned short rand_num = _rand.get_int(5);
    if(rand_num == 0 && _timer == 0){
        bn::sound_items::sb_dooj.play(_volume);
        _timer = 1;
    }
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

}

