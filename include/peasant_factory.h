#include <bn_forward_list.h>
#include <bn_vector.h>
#include <bn_random.h>
#include "cottage.h"
#include "peasant.h"

namespace trog{



    class peasant_factory { 
    private:

        unsigned short _time_since_last_spawn;
        unsigned short _next_spawn;

            
        bn::vector<cottage, 10>& _cottages; 
        bn::forward_list<peasant, 20>& _peasants;
        bn::random _random;
        void spawn_peasant();
        void reset_spawn_clock();
    public:
        peasant_factory(bn::vector<cottage, 10>& cottages, bn::forward_list<peasant, 20>& peasants);
        void update();

    };

}
