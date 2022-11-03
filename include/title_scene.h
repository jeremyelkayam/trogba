#pragma once
#include <bn_regular_bg_ptr.h>
#include "scene.h"
#include "bn_regular_bg_items_titlebg.h"
#include "bn_regular_bg_items_titlegraphic.h"

namespace trog{

    class title_scene : public scene{ 
    private:
        bn::regular_bg_ptr titlebg = bn::regular_bg_items::titlebg.create_bg(-3, 48);
        bn::regular_bg_ptr title = bn::regular_bg_items::titlegraphic.create_bg(10, 58);
        int frame_counter;
    public:
        title_scene();
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
