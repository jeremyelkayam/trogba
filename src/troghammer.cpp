#include <bn_log.h>

#include "troghammer.h"
#include "level_data.h"
#include "bn_sprite_items_troghammer.h"

namespace trog { 

//TODO: Make a saved_data class/struct so that we can save troghammer status

troghammer::troghammer(const bn::fixed_point &pos, bool facingRight, int level) :
    knight(pos.x(), pos.y(), facingRight),
    _waiting_time(120 SECONDS)
{
    //TODO: Make the troghammer bigger, he's not imposing enough
    _sprite = bn::sprite_items::troghammer.create_sprite(_pos.x(), _pos.y());
    _sprite.set_horizontal_flip(facingRight);
    _sprite.set_scale(0.25);
    _sprite.put_below();
    _sprite.set_visible(false);

    _walkcycle = bn::create_sprite_animate_action_forever(
                    _sprite, (20 / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer(), bn::sprite_items::troghammer.tiles_item(), 1, 3, 4, 3);
    _speed *= TROG_HAMMER_SPEEDUP_FACTOR;
    _cycletime = (bn::fixed(_cycletime) / TROG_HAMMER_SPEEDUP_FACTOR).floor_integer();

    //hitboxes are 20% bigger to make it harder
    _hitbox.set_width(TROG_HAMMER_WIDTH);
    _hitbox.set_height(TROG_HAMMER_HEIGHT);

    //for every 10 levels the waiting time gets 10% lower.
    _waiting_time -= _waiting_time * (level / 9) * 0.1;
 
    if(level > 90) _waiting_time = 0;
    _initial_waiting_time = _waiting_time;


    //TODO refactor this into a function
    int level_index;
    if (level == 1) {
		level_index = 0;
	} else {
		level_index = ((level - 2) % 32 + 2) - 1;
	}


    _time_of_day = levels[level_index][0];

}

void troghammer::update(){



     if(_waiting_time == 1){
        set_y(TROG_COUNTRYSIDE_TOP_BOUND + 10);
        set_x(0);
        _sprite.set_scale(1);
        _speed = -_speed;
     }

    if(_waiting_time > 0){
        --_waiting_time;

        if(_waiting_time == _initial_waiting_time * 2 / 3) _sprite.set_visible(true);
        
        if(_waiting_time <= _initial_waiting_time * 2 / 3){
            
            //update walking cycle
            set_x(_pos.x() + _speed * _sprite.vertical_scale());
            if(_pos.x().floor_integer() < 120 && 
                _pos.x().floor_integer() > -120){

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
            _walkcycle.update();

            //at 1/3 time, walk the other way
            if(_waiting_time == _initial_waiting_time / 3) {
                set_x(TROG_COUNTRYSIDE_RIGHT_BOUND);
                _sprite.set_scale(0.5);
                _sprite.set_horizontal_flip(!_sprite.horizontal_flip());
                _speed = -_speed;
            }
        }
    }else{
        knight::update();
    }
}


}