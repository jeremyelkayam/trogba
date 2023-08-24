#pragma once
#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include "bn_sprite_items_firebreath.h"
#include "entity.h"
#include "cottage.h"
#include "peasant.h"
#include "constants.h"
#include "session_info.h"
#include "common_stuff.h"

namespace trog {
    class firebreath : public entity { 
        private:
 
            bn::sprite_animate_action<4> _burningflames;
            session_info &_sesh;
            common_stuff &_common_stuff;
            bool _enabled;
            
        public:
            firebreath(session_info &sesh, common_stuff &common_stuff);
            virtual void update() final;
            void handle_cottage_collision(cottage &cottage);
            void handle_peasant_collision(peasant &peasant);
            void enable();
            void disable();
            bool enabled();
    };
}