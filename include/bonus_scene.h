#pragma once
#include <bn_regular_bg_ptr.h>
#include "scene.h"
#include "session_info.h"


namespace trog{

    class bonus_scene : public scene{ 
    private:
        bn::regular_bg_ptr _bg;

        session_info &_sesh;
    public:
        bonus_scene(session_info &sesh);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
