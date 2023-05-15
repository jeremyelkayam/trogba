#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_text_generator.h>
#include "scene.h"


#include "session_info.h"


namespace trog{

    class options_scene : public scene{ 
    private:

        common_stuff &_common_stuff;
        
        bn::vector<bn::sprite_ptr, 8> _header_sprites;
        bn::regular_bg_ptr _scroll;

        bool _go_to_credits;

    public:
        explicit options_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
