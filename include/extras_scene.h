#pragma once
#include "menu_scene.h"
#include "session_info.h"
#include "common_stuff.h"


namespace trog{

    class extras_scene : public menu_scene { 
    private:

    public:
        explicit extras_scene(common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };


}
