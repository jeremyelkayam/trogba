#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_palette_actions.h>
#include "scene.h"
#include "session_info.h"
#include "text_box.h"
#include "player.h"
#include "big_text.h"

namespace trog{
    struct dragon_option {
        dragon dragon_type;
        bn::string<16> name;
        bn::unique_ptr<player> player_entity;
    };

    class dragon_select_scene : public scene{ 
    private:
        uint8_t _index, _selection_timer;
        bn::vector<dragon_option,2> _selectable_dragons;
        bn::vector<bn::sprite_ptr, 4> _selected_text;

        big_text _title;


        session_info &_sesh;
        common_stuff &_common_stuff;

        void update_text();

    public:
        explicit dragon_select_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
