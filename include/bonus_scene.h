#pragma once
#include <bn_regular_bg_ptr.h>
#include "scene.h"
#include "session_info.h"
#include "player.h"


namespace trog{

    class bonus_scene : public scene{ 
    private:
        bn::regular_bg_ptr _bg;
        bn::fixed_rect _exit;
        player _trogdor;

        //ONLY NEEDED BECAUSE TROGDOR HAS A REFERENCE TO THE DAMN COTTAGE VECTOR
        //and there ARE NO COTTAGES on the bonus screen
        bn::vector<cottage, 10> _dummy;

        session_info &_sesh;
    public:
        bonus_scene(session_info &sesh);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
