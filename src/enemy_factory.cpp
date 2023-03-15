#include <bn_string.h>
#include "enemy_factory.h"

namespace trog
{
    enemy_factory::enemy_factory(int min, int max) : 
        _min_spawn_interval(min),
        _max_spawn_interval(max)
    {
        if(_min_spawn_interval == 0) {
            _min_spawn_interval = 60;
            _max_spawn_interval = 180;
        }
        reset_spawn_clock();
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
