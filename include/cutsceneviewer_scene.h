#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>

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
        bn::vector<cvoption, 14> _options_vec;
        bn::regular_bg_ptr _scroll;

        uint8_t _index;

        void update_selection();

    public:
        explicit cutsceneviewer_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
