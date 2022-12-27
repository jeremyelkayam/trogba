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

}

