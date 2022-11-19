#include <bn_log.h>
#include <bn_string.h>
#include "archer_factory.h"

namespace trog
{
    //todo: make it scale based on level
    archer_factory::archer_factory(bn::forward_list<archer, 4>& archers) : 
        enemy_factory(TROG_ARCHER_MIN_SPAWN_INTERVAL, TROG_ARCHER_MAX_SPAWN_INTERVAL),
        _archers(archers)
    {
        BN_LOG("new peasant factory");
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

            bn::fixed spawn_ycor = _random.get_int(TROG_ARCHER_MIN_SPAWN_HEIGHT, TROG_ARCHER_MAX_SPAWN_HEIGHT);

            _archers.emplace_front(spawn_ycor, direction);
        }
    }

}
