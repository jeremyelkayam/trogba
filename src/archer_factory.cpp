#include <bn_log.h>
#include <bn_string.h>
#include "archer_factory.h"

namespace trog
{
    //todo: make it scale based on level
    archer_factory::archer_factory(bn::forward_list<archer, 4>& archers, const unsigned short level, bn::random &random) : 
        enemy_factory(random, 
                    TROG_ARCHER_MIN_SPAWN_INTERVAL * (1 - (level / 10)*0.1), 
                    TROG_ARCHER_MAX_SPAWN_INTERVAL * (1 - (level / 10)*0.1)),
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
            BN_LOG("NO MORE SPACE FOR NEW ARCHERS. TRY AGAIN LATER");
        }
        else
        {
            // decide randomly which side he will come from
            // get_int will generate up to (but not including) the second value 
            // so this generates 0 or 1
            // 1 = left, 0 = right
            bool direction = _random.get_int(0, 2);
            BN_LOG("archer direction: ", direction);

            bn::fixed spawn_ycor = _random.get_int(TROG_ARCHER_MIN_SPAWN_HEIGHT, TROG_ARCHER_MAX_SPAWN_HEIGHT);

            _archers.emplace_front(spawn_ycor, direction);
        }
    }

}
