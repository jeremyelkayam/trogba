#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include <bn_display.h>
#include <bn_blending_transparency_attributes_hbe_ptr.h>
#include <bn_blending_transparency_attributes.h>

#include "scene.h"
#include "session_info.h"

namespace trog{

    struct cvoption {
        bn::vector<bn::sprite_ptr, 2> lv_text_sprites;
        bn::vector<bn::sprite_ptr, 6> title_text_sprites;
        bn::string<32> title;
        uint8_t level;
    };

    class cutsceneviewer_scene : public scene{ 
        //todo: refactor this into some superclass of options_scene
    private:
        session_info &_sesh;
        common_stuff &_common_stuff;
        
        bn::vector<bn::sprite_ptr, 8> _header_sprites;
        bn::vector<cvoption, 20> _options_vec;
        bn::regular_bg_ptr _scroll;
        bn::sprite_text_generator _black_generator;
        bn::sprite_palette_ptr _norm_palette, _selected_palette;
        bn::array<bn::blending_transparency_attributes, bn::display::height()> _trans_atts;
        bn::blending_transparency_attributes_hbe_ptr _bta_hbe;
        bn::sprite_ptr _left_icon, _right_icon, _head_icon;

        int _index;
        static constexpr const int SPACING = 12;
        static constexpr const int OPAQUE_START = 26;
        static constexpr const int OPAQUE_END = 143;
        static constexpr const int TRANSITION_LINES = 8;
        const bn::fixed _top_ycor;

        

        void update_selection();
        void create_line(bn::sprite_text_generator &gen, int index);

        void move_all(const bn::fixed &move_by);

    public:
        explicit cutsceneviewer_scene(session_info &sesh, common_stuff &common_stuff,
            const bn::fixed &top_ycor);
        bn::fixed top_ycor() const
        {
            return _options_vec.at(0).lv_text_sprites.at(0).y();
        }
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
