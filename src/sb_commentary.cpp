#include <bn_log.h>
#include "sb_commentary.h"

namespace trog { 

sb_commentary::sb_commentary(const bn::fixed &volume, bn::random &rand) : 
    _volume(volume), _rand(rand), _timer(0) {
}

const voice_clips &sb_commentary::get_clips(const dragon &dragon){
    switch(dragon){
        case dragon::TROGDOR:
            return all_clips[0];
        case dragon::SUCKS:
            return all_clips[1];
        case dragon::CHIAROSCURO:
            return all_clips[2];
        case dragon::WORMDINGLER:
            return all_clips[3];
        default:
            BN_ERROR("Invalid dragon passed into sb_commentary");
            return all_clips[0];
    }
}

void sb_commentary::play_sound(const bn::sound_item &item){
    if(_timer == 0 && item != bn::sound_items::dummy){
        item.play(_volume);
        _timer = 255;
    }else{
        BN_LOG("did not play id: ", item.id(), _timer ? "due to timer" : "due to dummy sound");
    }
}

bool sb_commentary::gameover(const unsigned int &score, const dragon &dragon){
    if(score >= TROG_GAMEOVER_SECRET_SCORE) {
        play_sound(get_clips(dragon).secretscore);
        return true;
    }else if(score >= TROG_GAMEOVER_BEAT_THAT_SCORE){
        play_sound(get_clips(dragon).highscore);
        return true;
    }else if(percent_chance(50)){
        play_sound(get_clips(dragon).gameover);
        return true;
    }
    return false;
}

void sb_commentary::arrowed(const dragon &dragon){
    if(percent_chance(33.33333)){
        play_sound(get_clips(dragon).arrowed);
    }
}

void sb_commentary::sworded(const dragon &dragon){
    if(percent_chance(33.33333)){
        play_random_sound(get_clips(dragon).sworded, 2);
    }
}

void sb_commentary::burninate(const dragon &dragon){
    if(percent_chance(50)){
        play_random_sound(get_clips(dragon).burninate, 3);
    }
}

void sb_commentary::level_win_pause(const dragon &dragon, const bool &troghammer_arrived){
    BN_LOG("troghammer in play? ", troghammer_arrived);
    if(troghammer_arrived) {
        play_sound(get_clips(dragon).majesty_troghammer);
    }else if(percent_chance(20))
        play_sound(get_clips(dragon).majesty);
}

void sb_commentary::level_win_scene(const dragon &dragon){
    if(percent_chance(20))
        play_random_sound(get_clips(dragon).levelbeat, 2);
}

void sb_commentary::stomp_peasant(const dragon &dragon){
    if(percent_chance(5))
        play_random_sound(get_clips(dragon).stomp, 3);
}

void sb_commentary::ignite_peasant(const dragon &dragon){
    if(percent_chance(20))
        play_random_sound(get_clips(dragon).ignitepeasant, 2);
}

void sb_commentary::ignite_cottage(const dragon &dragon){
    if(percent_chance(20))
        play_sound(get_clips(dragon).ignitecottage);
}

void sb_commentary::update(){
    if(_timer > 0){
        --_timer;
    }
}

bool sb_commentary::select_character(const dragon &dragon){
    if(percent_chance(25)){
        play_sound(get_clips(dragon).selection);
        return true;
    }
    return false;
}

void sb_commentary::play_random_sound(const bn::sound_item *item, const uint8_t &count){    
    uint8_t rand_num = _rand.get_int(count);
    play_sound(item[rand_num]);
}


bool sb_commentary::percent_chance(const bn::fixed &pct){
    BN_ASSERT(pct >= 0, "percent must be between 0 and 100");
    BN_ASSERT(pct < 100, "percent must be between 0 and 100");
    return pct > _rand.get_fixed(100);
} 

}

