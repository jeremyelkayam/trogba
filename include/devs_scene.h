#pragma once
#include <bn_sprite_ptr.h>
#include <bn_vector.h>
#include "scene.h"


namespace trog{

    class devs_scene : public scene{ 
    private:
        bn::vector<bn::sprite_ptr, 2> _butano;
        bn::vector<bn::sprite_ptr, 16> _butano_text;
        int _frame_counter;

    public:
        devs_scene();
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };
}
