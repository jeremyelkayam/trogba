#pragma once
#include <bn_regular_bg_ptr.h>
#include <bn_forward_list.h>
#include "scene.h"
#include "session_info.h"
#include "trogdor.h"
#include "moneybag.h"


namespace trog{

    class bonus_scene : public scene{
    private:
        bn::regular_bg_ptr _bg;
        bn::fixed_rect _exit;
        bn::unique_ptr<player> _player;

        bn::forward_list<moneybag, 10> _money;
        bn::forward_list<bn::fixed_rect, 5> _wall_hitboxes;


        static bool moneybag_deletable(moneybag &m) {return m.collected;}
        session_info &_sesh;



    public:
        bonus_scene(session_info &sesh);
        [[nodiscard]] virtual bn::optional<scene_type> update() final;

    };

}
