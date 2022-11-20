#include <bn_log.h>
#include <bn_string.h>
#include "peasant_factory.h"

namespace trog
{

    peasant_factory::peasant_factory(bn::vector<cottage, 10> &cottages, bn::forward_list<peasant, 20> &peasants) : 
        enemy_factory(TROG_PEASANT_MIN_SPAWN_INTERVAL, TROG_PEASANT_MAX_SPAWN_INTERVAL),
        _cottages(cottages),
        _peasants(peasants)
    {
        BN_LOG("new peasant factory");
    }

    void peasant_factory::spawn()
    {
        BN_LOG("spawn o'clock. number o' peasants pre-spawn: ", _peasants.size());
        if (_peasants.full())
        {
            BN_LOG("NO MORE SPACE FOR NEW PEASANTS. TRY AGAIN LATER");
        }
        else
        {
            // decide randomly which cottage the peasant will come from
            cottage &home = _cottages.at(_random.get_int(0, _cottages.size()));
            bn::fixed spawn_xcor = home.get_x();
            bn::fixed spawn_ycor = home.get_y();

            int max_dist;
            
            if (home.get_direction() == direction::LEFT)
            {
                //offset the spawn position
                spawn_xcor -= 10;
                spawn_ycor += 10;
                max_dist = spawn_xcor.integer() + 120;
            }
            else if (home.get_direction() == direction::RIGHT)
            {
                spawn_xcor += 10;
                spawn_ycor += 10;
                max_dist = 120 - spawn_xcor.integer();
            }
            else if (home.get_direction() == direction::UP)
            {
                spawn_ycor -= 10;
                max_dist = spawn_ycor.integer() - TROG_COUNTRYSIDE_TOP_BOUND;
            }
            else if (home.get_direction() == direction::DOWN)
            {
                spawn_ycor += 10;
                max_dist = TROG_COUNTRYSIDE_BOTTOM_BOUND - spawn_ycor.integer();
            }else BN_ASSERT(false, "invalid direction");

            bn::fixed dist = _random.get_int(TROG_PEASANT_MIN_WALK_DISTANCE, max_dist);


            _peasants.emplace_front(spawn_xcor, spawn_ycor, bn::fixed(TROG_PEASANT_SPEED), dist, home.get_direction());
        }
    }

}
