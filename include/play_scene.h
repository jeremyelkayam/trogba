#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "session_info.h"
#include "player.h"
#include "hud.h"
#include "cottage.h"

namespace trog{



    class play_scene : public scene{ 
    private:
        session_info& _sesh;
        player _trogdor;
        hud _hud;
        bn::regular_bg_ptr _countryside;
        cottage _cottages; // make this a vector later
    public:
        play_scene(session_info& sesh, bn::sprite_text_generator& generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
