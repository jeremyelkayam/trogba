#include "session_info.h"
#include "constants.h"


namespace trog {

session_info::session_info() {

    // _cutscene_levels.insert(100);
    // for(int z : {4, 8, 12, 16, 20, 24, 30, 34, 38, 42, 46, 50, 100}){
    //     _cutscene_levels.insert(z);
    // }
    reset();
}

void session_info::score(unsigned int num_points){
    //once we cross 200 points, you get a 1up
    //we can see what multiple of 200 you're on with division
    int old_multiple = _score/TROG_POINTS_FOR_1UP;

    _score += num_points;

    int new_multiple = _score/TROG_POINTS_FOR_1UP;

    //if the multiple is higher after adding the points, add however many lives you earned
    _mans += (new_multiple - old_multiple);    
}

void session_info::reset(){
    _mans = TROG_STARTING_LIVES;
    _score = 0;
    _level = TROG_STARTING_LEVEL;
    _flagbyte = 'T'; // for TROGDOR.
}
bool session_info::is_valid_object(){
    return _flagbyte == 'T';
}

// THERE HAS TO BE A BETTER WAY!!!! 
bool session_info::current_level_has_cutscene(){
    return  (_level == 5) || 
            (_level == 9) ||
            (_level == 13) ||
            (_level == 17) ||
            (_level == 21) ||
            (_level == 25) ||
            (_level == 31) ||
            (_level == 35) ||
            (_level == 39) ||
            (_level == 43) ||
            (_level == 47) ||
            (_level == 51) ||
            (_level == 101);
}

}