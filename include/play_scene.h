#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_forward_list.h>
#include "scene.h"
#include "session_info.h"
#include "player.h"
#include "hud.h"
#include "cottage.h"
#include "peasant_factory.h"
#include "peasant.h"

namespace trog{



    class play_scene : public scene{ 
    private:
        
        session_info& _sesh;
        player _trogdor;
        hud _hud;
        peasant_factory _pfact;

        bn::regular_bg_ptr _countryside;

        bn::vector<cottage, 10> _cottages; 
        bn::forward_list<peasant, 20> _peasants;
        bool level_complete();

        bool peasant_deletable(peasant &p) {return p.remove_from_map();};

    public:
        play_scene(session_info& sesh, bn::sprite_text_generator& generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
