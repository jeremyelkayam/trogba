#pragma once

#include <bn_forward_list.h>
#include <bn_vector.h>
#include <bn_random.h>

namespace trog{

    class enemy_factory { 
    protected:

        unsigned short _time_since_last_spawn;
        unsigned short _next_spawn;
        unsigned short _min_spawn_interval;
        unsigned short _max_spawn_interval;

        bn::random _random;
        virtual void spawn() = 0;
        void reset_spawn_clock();
    public:
        virtual ~enemy_factory() = default;
        enemy_factory(int min, int max);
        void update();

    };

}
