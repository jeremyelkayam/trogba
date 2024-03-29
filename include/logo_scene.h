#pragma once
#include <bn_random.h>
#include <bn_regular_bg_ptr.h>
#include <bn_bgs_mosaic_actions.h>
#include "trogdor.h"
#include "common_stuff.h"
#include "scene.h"

namespace trog{

    class logo_scene : public scene{ 
    private:
        bn::regular_bg_ptr _bg;
        bn::optional<bn::bgs_mosaic_stretch_to_action> _mosaic_action;
        int _frame_counter;
        bool _easter_egg;

        trogdor _trogdor;

    public:
        logo_scene(session_info &sesh, common_stuff &common_stuff);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;
    };

}
