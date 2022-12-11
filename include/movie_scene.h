#pragma once
#include <bn_sprite_text_generator.h>
#include <bn_sprite_ptr.h>
#include "scene.h"
#include "session_info.h"


namespace trog{

    class movie_scene : public scene{ 
    private:
        bn::vector<bn::sprite_ptr, 20> _text_sprites;
        
        session_info &_sesh;
    public:
        explicit movie_scene(session_info &sesh, bn::sprite_text_generator &text_generator);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
