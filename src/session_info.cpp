#include "session_info.h"


namespace trog {

session_info::session_info() {
    reset();
}

void session_info::score(unsigned int num_points){
    //once we cross 300 points, you get a 1up
    //we can see what multiple of 300 you're on with division
    int old_multiple = _score/300;

    _score += num_points;

    int new_multiple = _score/300;

    //if the multiple is higher after adding the points, add however many lives you earned
    _mans += (new_multiple - old_multiple);    
}

void session_info::reset(){
    _mans = 3;
    _score = 0;
    _level = 1;
}

void session_info::set_level(unsigned short level){
    _level = level;
}

}