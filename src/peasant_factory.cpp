#include <bn_log.h>
#include <bn_string.h>
#include "peasant_factory.h"

namespace trog
{

    peasant_factory::peasant_factory(bn::vector<cottage, 10> &cottages, bn::forward_list<peasant, 20> &peasants) : _cottages(cottages),
                                                                                                                   _peasants(peasants)
    {
        reset_spawn_clock();
    }

    void peasant_factory::update()
    {
        ++_time_since_last_spawn;
        if (_time_since_last_spawn == _next_spawn)
        {
            reset_spawn_clock();
            spawn_peasant();
        }
    }

    void peasant_factory::spawn_peasant()
    {
        BN_LOG("spawn o'clock. number o' peasants pre-spawn: ", _peasants.size());
        if (_peasants.full())
        {
            BN_LOG("NO MORE SPACE FOR NEW PEASANTS. TRY AGAIN LATER");
        }
        else
        {
            // decide randomly which cottage the peasantwill come from
            cottage &home = _cottages.at(_random.get_int(0, _cottages.size()));
            if (home.get_direction() == direction::LEFT)
            {
                BN_LOG("home cottage direction: left");
            }
            else if (home.get_direction() == direction::RIGHT)
            {
                BN_LOG("home cottage direction: right");
            }
            else if (home.get_direction() == direction::UP)
            {
                BN_LOG("home cottage direction: up");
            }
            else if (home.get_direction() == direction::DOWN)
            {
                BN_LOG("home cottage direction: down");
            }

            _peasants.emplace_front(home.get_x(), home.get_y(), bn::fixed(0.5), bn::fixed(90), home.get_direction());
        }
    }

    void peasant_factory::reset_spawn_clock()
    {
        _time_since_last_spawn = 0;
        _next_spawn = _random.get_int(TROG_PEASANT_MIN_SPAWN_INTERVAL, TROG_PEASANT_MAX_SPAWN_INTERVAL);
    }
}
