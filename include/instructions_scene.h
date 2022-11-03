#pragma once
#include <bn_regular_bg_ptr.h>
#include "scene.h"
#include "bn_regular_bg_items_titlegraphic.h"

namespace trog{

    class instructions_scene : public scene{ 
    private:
        bn::regular_bg_ptr title = bn::regular_bg_items::titlegraphic.create_bg(10, 58);
    public:
        instructions_scene();
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
