#include <bn_log.h>

#include "troghammer.h"
#include "level_data.h"
#include "bn_sprite_items_troghammer.h"

namespace trog { 

//TODO: Make a saved_data class/struct so that we can save troghammer status

troghammer::troghammer(const bn::fixed_point &pos, bool facingRight, int level) :
    knight(pos.x(), pos.y(), facingRight),
    _total_wait_time(120 SECONDS)
{
    _sprite = bn::sprite_items::troghammer.create_sprite(_pos.x(), _pos.y());
    _sprite.set_horizontal_flip(facingRight);
    _sprite.set_scale(0.25);
    _sprite.put_below();
    _sprite.set_visible(false);


    _walkcycle = bn::create_sprite_animate_action_forever(
                    _sprite, (20 / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer(), bn::sprite_items::troghammer.tiles_item(), 1, 3, 4, 3);

    // speed the troghammer up a bit
    _speed *= TROG_HAMMER_SPEEDUP_FACTOR;
    _cycletime = (bn::fixed(_cycletime) / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer();

    //hitboxes are bigger to make it harder
    _hitbox.set_width(TROG_HAMMER_WIDTH);
    _hitbox.set_height(TROG_HAMMER_HEIGHT);

    //for every 10 levels the waiting time gets 10% lower.
    _total_wait_time -= _total_wait_time * (level / 9) * 0.1;
    _states.push_back(troghammer_state::ARRIVED);


    if(_total_wait_time > 50 SECONDS){
        _states.push_back(troghammer_state::COMING);
    }
    if(_total_wait_time > 25 SECONDS){
        _states.push_back(troghammer_state::AWAKE);
    }
    if(_total_wait_time > 0 SECONDS){
        _states.push_back(troghammer_state::ALERT);
    }

    _waiting_time_per_state = _total_wait_time / _states.size();

    advance_to_next_state();




    //TODO refactor this into a function
    int level_index;
    if (level == 1) {
		level_index = 0;
	} else {
		level_index = ((level - 2) % 32 + 2) - 1;
	}
    _time_of_day = levels[level_index][0];


}

void troghammer::advance_to_next_state(){
    _new_state = true;
    _current_state = _states.back();
    _states.pop_back();
    _timer = 0;
}

void troghammer::init_current_state(){
    BN_ASSERT(_new_state);

    if(_current_state == troghammer_state::ARRIVED){
        _sprite.set_visible(true);
        //spawn at the top of the screen
        unsigned int spawnpos = rand() % 4;
        BN_LOG("spawn position: ", spawnpos);
        //problem: the troghammer can spawn on top of you
        switch(spawnpos){
            case 0:
                set_y(TROG_COUNTRYSIDE_TOP_BOUND + 10);
                set_x(0);
                break;
            case 1:
                set_y(TROG_COUNTRYSIDE_BOTTOM_BOUND - 10);
                set_x(0);
                break;
            case 2:
                set_y(0);
                set_x(TROG_COUNTRYSIDE_LEFT_BOUND + 10);
                break;
            case 3:
                set_y(0);
                set_x(TROG_COUNTRYSIDE_RIGHT_BOUND - 10);
                break;
            default:
                BN_ERROR("Invalid spawn position for troghammer");
        }
        _sprite.set_scale(1);

    }else if(_current_state == troghammer_state::ALERT){
        _sprite.set_visible(false);
    }else if(_current_state == troghammer_state::AWAKE){
        _sprite.set_visible(true);
    }else if(_current_state == troghammer_state::COMING){
        _sprite.set_visible(true);
        set_x(TROG_COUNTRYSIDE_RIGHT_BOUND);
        _sprite.set_scale(0.5);
        _sprite.set_horizontal_flip(!_sprite.horizontal_flip());
    }
}

void troghammer::update(){
    if(_new_state){
        init_current_state();
        _new_state = false;
    }


    if(_current_state == troghammer_state::ARRIVED){
        knight::update();
    }else{
        ++_timer;

        if(_current_state == troghammer_state::AWAKE || 
           _current_state == troghammer_state::COMING){
            bn::fixed delta_x = _speed * _sprite.vertical_scale();
            
            if(_current_state == troghammer_state::COMING){
                delta_x = -delta_x;
            }

            set_x(_pos.x() + delta_x);
            if(_pos.x().floor_integer() < 120 && 
                _pos.x().floor_integer() > -120){
                set_ycor_to_horizon();
            }
            _walkcycle.update();
        }

        BN_LOG("time until next troghammer state: ", _timer, "/", _waiting_time_per_state);

        if(_timer == _waiting_time_per_state){
            advance_to_next_state();
        }
    }

    
    
}

void troghammer::set_ycor_to_horizon(){
    bn::fixed ypos;

    switch(_time_of_day){
        case 1:
            ypos = day_path[_pos.x().floor_integer() + 120];
            break;
        case 2:
            ypos = dusk_path[_pos.x().floor_integer() + 120];
            break;
        case 3:
            ypos = night_path[_pos.x().floor_integer() + 120];
            break;
        case 4:
            ypos = dawn_path[_pos.x().floor_integer() + 120];
            break;
        default:
            BN_ERROR("invalid time of day in level_data.h");
    }
    set_y(ypos);
}

troghammer_status troghammer::get_status(){
    troghammer_status result = {_current_state, _timer, _pos};
    return result;
}

troghammer::troghammer(troghammer_status status, bool facingRight, int level) : 
    troghammer(status.pos, facingRight, level) {
    while(_current_state != status.current_state){ 
        _current_state = _states.back(); 
        _states.pop_back();
        BN_LOG("state: ");
        log_state(_current_state);
        BN_LOG("loaded state: ");
        log_state(status.current_state);
    }
    
    _timer = status.timer;
}

void troghammer::log_state(troghammer_state state){
    switch(state){
        case troghammer_state::ALERT:
            BN_LOG("alert");
            break;
        case troghammer_state::ARRIVED:
            BN_LOG("arrived");
            break;        
        case troghammer_state::COMING:
            BN_LOG("coming");
            break;
        case troghammer_state::AWAKE:
            BN_LOG("awake");
            break;
        case troghammer_state::UNALERTED:
            BN_LOG("unalerted");
            break;
        default:
            BN_LOG("unknown state");
    }
}


}