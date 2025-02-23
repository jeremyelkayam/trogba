#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include "menu_scene.h"
#include "session_info.h"


namespace trog{

    class main_menu_scene : public menu_scene { 
    private:

        unsigned int _last_line2_option;

        bn::sprite_ptr _cursor, _flames, _troghammer_icon, _trogmeter_degrade_icon;
        bn::sprite_animate_action<4> _flames_anim;
        bn::sprite_scale_to_action _flames_scale;
        bn::optional<bn::sprite_move_to_action> _flames_translate;
        
        unsigned int _selection_anim_timer;

        session_info &_sesh;

        bn::fixed _scroll_speed;

        void select();

    public:
        explicit main_menu_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };


}
