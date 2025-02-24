#pragma once
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"
#include "common_stuff.h"



namespace trog{

    class achievements_scene : public scene{ 
    private:

        bn::vector<bn::sprite_ptr, 64> _icons;
        bn::vector<bn::sprite_ptr, 32> _text_sprites;
        bn::sprite_ptr _selected_icon, _selector;
        bn::regular_bg_ptr _bg;

        common_stuff &_common_stuff;

        unsigned int _selected;

        static constexpr const bn::fixed_point menu_top_left = 
            bn::fixed_point(-82, -25);
        static constexpr const bn::fixed spacing = 22;

    public:
        explicit achievements_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
