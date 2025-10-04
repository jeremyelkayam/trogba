#include <bn_string.h>
#include "enemy_factory.h"

namespace trog
{
    enemy_factory::enemy_factory(bn::random &random, int min, int max) : 
        _time_since_last_spawn(0),
        _min_spawn_interval(min),
        _max_spawn_interval(max),
        _random(random)
    {
        if(_min_spawn_interval == 0) {
            _min_spawn_interval = 60;
            _max_spawn_interval = 180;
        }
        //start at 0 just so that there's no spawning 
        // pause at the beginning of the level 
        _next_spawn = _random.get_int(0, _max_spawn_interval);
    }

    void enemy_factory::update()
    {
        ++_time_since_last_spawn;
        if (_time_since_last_spawn == _next_spawn)
        {
            reset_spawn_clock();
            spawn();
        }
    }

    void enemy_factory::reset_spawn_clock()
    {
        _time_since_last_spawn = 0;
        _next_spawn = _random.get_int(_min_spawn_interval, _max_spawn_interval);
    }
}
