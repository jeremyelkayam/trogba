#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>
#include <bn_sprite_ptr.h>
#include "scene.h"


namespace trog{

    class title_scene : public scene{ 
    private:
        bn::regular_bg_ptr _titlebg;
        bn::vector<bn::sprite_ptr, 4> _title_sprites;
        bn::sprite_ptr _version_label;
        
        int _frame_counter;
    public:
        title_scene();
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
