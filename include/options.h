#pragma once
#include <bn_fixed.h>

namespace trog{
    
    class options { 
    private:
        bn::fixed _sound_vol, _music_vol, _voice_vol;
        bool _troghammer, _decrement_trogmeter;
        uint8_t _starting_lives;
    public:

        options() {init();}
        bn::fixed voice_vol() {return _voice_vol;}

        void init() { _sound_vol = 0.75; _music_vol = 0.75; _voice_vol = 0.75; 
                      _troghammer = false; _decrement_trogmeter = false;
                      _starting_lives = 3;}

        void save() {}
        void load() {}
    };

    static options opts;
    
}
