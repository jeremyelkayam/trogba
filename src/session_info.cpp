#include "session_info.h"
#include "constants.h"


namespace trog {

session_info::session_info(hud &hud) :
    _hud(hud) {
    reset();
}

void session_info::score(unsigned int num_points){
    //once we cross 300 points, you get a 1up
    //we can see what multiple of 300 you're on with division
    int old_multiple = _score/TROG_POINTS_FOR_1UP;

    _score += num_points;
    _hud.update_score(_score);

    int new_multiple = _score/TROG_POINTS_FOR_1UP;

    //if the multiple is higher after adding the points, add however many lives you earned
    _mans += (new_multiple - old_multiple);    
    if(new_multiple > old_multiple){
        _hud.update_mans_lv(_mans, _level);
    }
}

void session_info::reset(){
    _mans = TROG_STARTING_LIVES;
    _score = 0;
    _level = TROG_STARTING_LEVEL;

    _hud.update_score(_score);
    _hud.update_mans_lv(_mans, _level);

    _killed_by_archer = false;
    _visited_treasure_hut = false;
    _troghammer = true;

}

void session_info::set_level(unsigned short level){
    _visited_treasure_hut = false;
    _level = level;

    _hud.update_mans_lv(_mans, _level);
}

void session_info::visit_treasure_hut(){
    _visited_treasure_hut = true;
}

bool session_info::can_visit_treasure_hut(){
    return !_visited_treasure_hut;
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