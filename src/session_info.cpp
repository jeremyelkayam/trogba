#include "session_info.h"
#include "constants.h"


namespace trog {

session_info::session_info(common_stuff &common_stuff) : 
    _common_stuff(common_stuff) {
    reset();
}

void session_info::score(const unsigned int &num_points){
    _common_stuff.savefile.stats.points_earned += num_points;


    //once we cross 300 points, you get a 1up
    //we can see what multiple of 300 you're on with division
    int old_multiple = _score/TROG_POINTS_FOR_1UP;

    _score += num_points;

    int new_multiple = _score/TROG_POINTS_FOR_1UP;

    if(new_multiple - old_multiple)
    {
        
        _common_stuff.acm.update_achievement("1up");
        //if the multiple is higher after adding the points, add however many lives you earned
        _mans += (new_multiple - old_multiple);    
    }

}

void session_info::reset(){
    _mans = _common_stuff.savefile.options.starting_lives;
    _score = 0;
    _level = TROG_STARTING_LEVEL;

    _killed_by_archer = false;
    _troghammer = _common_stuff.savefile.options.troghammer;
    _can_lose_trogmeter = _common_stuff.savefile.options.decrement_trogmeter;
    _dragon = dragon::TROGDOR;
}

void session_info::import_save(){
    saved_session &sesh = _common_stuff.savefile.session;
    BN_ASSERT(sesh.exists, "Session must exist to be loaded.");
    _mans = sesh.mans;
    _score = sesh.score;
    _level = sesh.level;
    _troghammer = sesh.troghammer;
    _can_lose_trogmeter = sesh.can_lose_trogmeter;
    _dragon = sesh.current_dragon;
}

saved_session session_info::export_save(){
    saved_session result;
    result.exists = true;
    result.mans = _mans;
    result.score = _score;
    result.level = _level;
    result.can_lose_trogmeter = _can_lose_trogmeter;
    result.troghammer = _troghammer;
    result.current_dragon = _dragon;
    result.cottage_burnination_status = {false, false, false, 
                                         false, false, false};
    result.can_visit_treasure_hut = false;
    result.thinfo = {troghammer_state::UNALERTED, 0, bn::fixed_point(0, 0)};
    return result;
}

void session_info::set_level(const unsigned short &level){
    _level = level;
}



}