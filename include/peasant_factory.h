#include <bn_forward_list.h>
#include <bn_vector.h>
#include <bn_random.h>
#include "cottage.h"
#include "peasant.h"
#include "enemy_factory.h"

namespace trog{
    class peasant_factory : public enemy_factory { 
    private:
            
        cottage &_cottage; 
        bn::forward_list<peasant, 20>& _peasants;
        virtual void spawn() final;

    public:
        peasant_factory(cottage& cottage, 
            bn::forward_list<peasant, 20>& peasants, bn::random &rand);
    };

}
