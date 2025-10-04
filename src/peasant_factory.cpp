#include <bn_string.h>
#include "peasant_factory.h"

namespace trog
{
    peasant_factory::peasant_factory(cottage &cottage, 
            bn::forward_list<peasant, 20> &peasants, bn::random &rand) : 
        enemy_factory(rand, 120, 
            900),
        _cottage(cottage),
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
            bn::fixed spawn_xcor = _cottage.get_x();
            bn::fixed spawn_ycor = _cottage.get_y();

            int max_dist;
            
            if (_cottage.get_direction() == direction::LEFT)
            {
                //offset the spawn position
                spawn_xcor -= 10;
                spawn_ycor += 10;
                max_dist = spawn_xcor.integer() + 120;
            }
            else if (_cottage.get_direction() == direction::RIGHT)
            {
                spawn_xcor += 10;
                spawn_ycor += 10;
                max_dist = 120 - spawn_xcor.integer();
            }
            else if (_cottage.get_direction() == direction::UP)
            {
                spawn_ycor -= 10;
                max_dist = spawn_ycor.integer() - TROG_COUNTRYSIDE_TOP_BOUND - 10;
            }
            else if (_cottage.get_direction() == direction::DOWN)
            {
                spawn_ycor += 10;
                max_dist = TROG_COUNTRYSIDE_BOTTOM_BOUND - spawn_ycor.integer();
            }else BN_ASSERT(false, "invalid direction");

            bn::fixed dist = _random.get_int(TROG_PEASANT_MIN_WALK_DISTANCE, max_dist);


            _peasants.emplace_front(spawn_xcor, spawn_ycor, 
                TROG_PEASANT_SPEED, dist, _cottage);
        }
    }

}
