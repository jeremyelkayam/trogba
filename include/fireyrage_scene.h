#pragma once

#include <bn_fixed.h>
#include <bn_affine_bg_ptr.h>
#include <bn_affine_bg_pa_register_hbe_ptr.h>
#include <bn_affine_bg_pc_register_hbe_ptr.h>
#include <bn_affine_bg_dx_register_hbe_ptr.h>
#include <bn_affine_bg_dy_register_hbe_ptr.h>
#include <bn_display.h>
#include "scene.h"


namespace trog {
    struct camera
    {
        bn::fixed x = 440;
        bn::fixed y = 128;
        bn::fixed z = 320;
        int phi = 0;
        int cos = 0;
        int sin = 0;
    };
    class fireyrage_scene : public scene { 
        private:
            void update_camera();
            void update_hbe_values();

            camera _camera;
            bn::affine_bg_ptr _bg;
            int16_t _pa_values[bn::display::height()];
            bn::affine_bg_pa_register_hbe_ptr _pa_hbe;
            int16_t _pc_values[bn::display::height()];
            bn::affine_bg_pc_register_hbe_ptr _pc_hbe;

            int _dx_values[bn::display::height()];
            bn::affine_bg_dx_register_hbe_ptr _dx_hbe;
            int _dy_values[bn::display::height()];
            bn::affine_bg_dy_register_hbe_ptr _dy_hbe;

            
        public:
            fireyrage_scene();
            [[nodiscard]] virtual bn::optional<scene_type> update() final;    
    };
}