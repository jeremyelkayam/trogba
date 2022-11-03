#pragma once
#include <bn_regular_bg_ptr.h>
#include "scene.h"


namespace trog{

    class title_scene : public scene{ 
    private:
        bn::regular_bg_ptr titlebg;
        bn::regular_bg_ptr title;
        int frame_counter;
    public:
        title_scene();
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
