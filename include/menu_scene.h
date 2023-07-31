#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include "scene.h"
#include "session_info.h"
#include "common_stuff.h"


namespace trog{

    class menu_option {
        private:
            bn::vector<bn::sprite_ptr, 4> _text_sprites;
            bn::vector<bn::sprite_ptr, 4> _drop_shadow_sprites;
            bn::vector<bn::sprite_ptr, 4> _red_text_sprites;
        public:
            menu_option(const bn::fixed &x, const bn::fixed &y, const char *text, bn::sprite_text_generator& _text_generator);
            bn::fixed y() const {return _text_sprites.at(0).y();}
            void turn_red();
            void turn_white();
            void set_y(const bn::fixed &y);
    };
    class menu_scene : public scene{ 
    private:

        bn::vector<menu_option, 7> _menu_options;

        common_stuff & _common_stuff;

        bn::vector<bn::sprite_ptr, 64> _menu_text_sprites;
        bn::sprite_ptr _cursor, _flames, _troghammer_icon, _trogmeter_degrade_icon;
        bn::sprite_animate_action<4> _flames_anim;
        bn::sprite_scale_to_action _flames_scale;
        bn::regular_bg_ptr _bg;
        bn::optional<bn::sprite_move_to_action> _flames_translate;
        
        uint8_t _selected_option_index, _selection_anim_timer;

        session_info &_sesh;

        bn::fixed _scroll_speed;

        void select();

    public:
        explicit menu_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };


}
