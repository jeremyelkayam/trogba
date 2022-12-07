#pragma once 

#include <bn_forward_list.h>
#include <bn_vector.h>
#include <bn_random.h>
#include "archer.h"
#include "enemy_factory.h"

namespace trog{
    class archer_factory : public enemy_factory { 
    private:

        bn::forward_list<archer, 4>& _archers;
        virtual void spawn() final;

    public:
        archer_factory(bn::forward_list<archer, 4>& archers, const unsigned short level);
    };

}
