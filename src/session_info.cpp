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
}

bool session_info::current_level_has_cutscene(){
    return _level - 1 == 100;
}

}