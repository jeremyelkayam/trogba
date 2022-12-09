/* archer_factory.h
 * Author: Jeremy Elkayam
 * Purpose: Manage the random creation of archers 
 * 
 */

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

        /*
         * Spawn a new archer into the archer array.
         */
        virtual void spawn() final;

    public:
        /**
         * Constructor for archer_factory.
         * 
         * @param archers   A preallocated list containing the archers 
         *                  spawned by the factory.
         * @param level     The level currently being played.
         */
        archer_factory(bn::forward_list<archer, 4>& archers, const unsigned short level);
    };

}
