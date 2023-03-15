#include <bn_string.h>
#include "peasant_factory.h"

namespace trog
{
    peasant_factory::peasant_factory(bn::vector<cottage, 6> &cottages, bn::forward_list<peasant, 20> &peasants) : 
        enemy_factory(40, 120),
        _cottages(cottages),
        _peasants(peasants)
    {
    }

    void peasant_factory::spawn()
    {
        if (_peasants.full())
        {
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
                max_dist = spawn_ycor.integer() - -66 - 10;
            }
            else if (home.get_direction() == direction::DOWN)
            {
                spawn_ycor += 10;
                max_dist = 79 - spawn_ycor.integer();
            }else BN_ASSERT(false, "invalid direction");

            bn::fixed dist = _random.get_int(2, max_dist);


            _peasants.emplace_front(spawn_xcor, spawn_ycor, 0.435, dist, home.get_direction());
        }
    }

}
