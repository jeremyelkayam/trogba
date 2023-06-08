#include <bn_string.h>
#include "archer_factory.h"

namespace trog
{
    //todo: make it scale based on level
    archer_factory::archer_factory(bn::forward_list<archer, 4>& archers, const unsigned short level, bn::random &random) : 
        enemy_factory(15*60 * (1 - (level / 10)*0.1), 
                    30*60 * (1 - (level / 10)*0.1), random),
        _archers(archers)
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
            //do nothing
        }
        else
        {
            bool direction = _random.get_int(0, 2);

            bn::fixed spawn_ycor = _random.get_int(-65, 70);

            _archers.emplace_front(spawn_ycor, direction);
        }
    }

}
