#include "session_info.h"
#include "constants.h"


namespace trog {

session_info::session_info() {
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

}