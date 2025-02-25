#pragma once
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_actions.h>
#include "scene.h"
#include "common_stuff.h"
#include "achievement_rom_data.h"



namespace trog{

    struct ac_option
    {
        const ext::achievement_rom_data &data;
        bn::sprite_ptr icon;
        int index;
    };

    class achievements_scene : public scene{ 
    private:

        bn::vector<ac_option, 64> _opts;
        bn::vector<bn::sprite_ptr, 32> _text_sprites;
        bn::sprite_ptr _selected_icon, _selector;
        bn::sprite_animate_action<8> _selector_anim;
        bn::optional<bn::sprite_move_to_action> _selector_move;
        bn::regular_bg_ptr _bg;

        common_stuff &_common_stuff;

        int _selected;

        static constexpr const bn::fixed_point menu_top_left = 
            bn::fixed_point(-78, -21);
        static constexpr const int items_per_line = 6;
        static constexpr const bn::fixed spacing = 
            (-menu_top_left.x() * 2) / items_per_line; 

        void update_info_box();

    public:
        explicit achievements_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
