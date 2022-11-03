#pragma once
#include <bn_regular_bg_ptr.h>
#include "scene.h"
#include "player.h"

namespace trog{

    typedef struct session_info {
        unsigned short mans;
        unsigned int points;
    };

    class play_scene : public scene{ 
    private:
        session_info& sesh;
        player trogdor;
        bn::regular_bg_ptr countryside;
    public:
        play_scene(session_info& a_sesh);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
