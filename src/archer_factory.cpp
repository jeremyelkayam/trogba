#include <bn_string.h>
#include "archer_factory.h"

namespace trog
{
    archer_factory::archer_factory(bn::forward_list<archer, 4>& archers, const unsigned short &level, common_stuff &common_stuff) : 
        enemy_factory(common_stuff.rand, 
                    TROG_ARCHER_MIN_SPAWN_INTERVAL * (1 - (level / 10)*0.1), 
                    TROG_ARCHER_MAX_SPAWN_INTERVAL * (1 - (level / 10)*0.1)),
        _archers(archers),
        _common_stuff(common_stuff)
    {

        if(_min_spawn_interval == 0){
            _min_spawn_interval = 1*60;
            _max_spawn_interval = 5*60;
        }
    }

    void archer_factory::spawn()
    {
        if (_archers.full())
        {
        }
        else
        {
            // decide randomly which side he will come from
            // get_int will generate up to (but not including) the second value 
            // so this generates 0 or 1
            // 1 = left, 0 = right
            bool direction = _random.get_int(0, 2);

            bn::fixed spawn_ycor = _random.get_int(TROG_ARCHER_MIN_SPAWN_HEIGHT, TROG_ARCHER_MAX_SPAWN_HEIGHT);

            _archers.emplace_front(spawn_ycor, direction, _common_stuff);
        }
    }

}
