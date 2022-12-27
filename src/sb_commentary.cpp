#include <bn_log.h>
#include "sb_commentary.h"

namespace trog { 

sb_commentary::sb_commentary() {
}

bool sb_commentary::gameover(unsigned int score){
    if(score >= TROG_GAMEOVER_SECRET_SCORE) {
        bn::sound_items::sb_secretthing.play(_volume);        
        return true;
    }else if(score >= TROG_GAMEOVER_BEAT_THAT_SCORE){
        bn::sound_items::sb_beatthatscore.play(_volume);
        return true;
    }else if(rand() % 2 == 0){
        bn::sound_items::sb_pineforyou.play(_volume);
        return true;
    }
    return false;
}

void sb_commentary::arrowed(){
    if(rand() % 2 == 0){
        bn::sound_items::sb_stupid_archdeluxe.play(_volume);
    }
}

void sb_commentary::sworded(){
    unsigned short rand_num = rand() % 4;
    if(rand_num == 0){
        bn::sound_items::sb_stupidknight.play(_volume);
    }else if(rand_num == 1){
        bn::sound_items::sb_worstgame.play(_volume);
    }
}

void sb_commentary::burninate(){
    unsigned short rand_num = rand() % 4;
    if(rand_num == 0){
        bn::sound_items::sb_tablesturn.play(_volume);
    }else if(rand_num == 1){
        bn::sound_items::sb_advantage.play(_volume);
    }
}

void sb_commentary::level_win_pause(){
    unsigned short rand_num = rand() % 4;
    if(rand_num == 0){
        bn::sound_items::sb_ohyeah.play(_volume);
    }
}
void sb_commentary::level_win_scene(){
    unsigned short rand_num = rand() % 4;
    if(rand_num == 0){
        bn::sound_items::sb_comesinthenight.play(_volume);
    }else if(rand_num == 1){
        bn::sound_items::sb_bestgame.play(_volume);
    }
}

void sb_commentary::stomp_peasant(){
    unsigned short rand_num = rand() % 20;
    if(rand_num == 0){
        bn::sound_items::sb_asquisha.play(_volume);
    }else if(rand_num == 1){
        bn::sound_items::sb_asquishasquisha.play(_volume);
    }else if(rand_num == 2){
        bn::sound_items::sb_squishedyou.play(_volume);
    }
}

void sb_commentary::ignite_peasant(){
    unsigned short rand_num = rand() % 4;
    if(rand_num == 0){
        bn::sound_items::sb_hesonfire.play(_volume);
    }else if(rand_num == 1){
        bn::sound_items::sb_lookit_him_burning.play(_volume);
    }
}

void sb_commentary::ignite_cottage(){
    unsigned short rand_num = rand() % 4;
    if(rand_num == 0){
        bn::sound_items::sb_dooj.play(_volume);
    }
}

}

