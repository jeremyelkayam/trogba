/*
 * based on Mode 7 demo by Gustavo Valiente
 * Copyright (c) 2020-2023 Gustavo Valiente gustavo.valiente@protonmail.com
 * zlib License, see LICENSE file.
 */

#include <bn_math.h>
#include <bn_keypad.h>


#include "bn_affine_bg_items_ground.h"
#include "fireyrage_scene.h"

namespace trog
{


void fireyrage_scene::update_camera() {
    bn::fixed dir_x = 0;
    bn::fixed dir_z = 0;

    if(bn::keypad::left_held())
    {
        dir_x -= bn::fixed::from_data(32);
    }
    else if(bn::keypad::right_held())
    {
        dir_x += bn::fixed::from_data(32);
    }

    if(bn::keypad::down_held())
    {
        dir_z += bn::fixed::from_data(32);
    }
    else if(bn::keypad::up_held())
    {
        dir_z -= bn::fixed::from_data(32);
    }

    // if(bn::keypad::b_held())
    // {
    //     _camera.y -= bn::fixed::from_data(2048);

    //     if(_camera.y < 0)
    //     {
    //         _camera.y = 0;
    //     }
    // }
    // else if(bn::keypad::a_held())
    // {
    //     _camera.y += bn::fixed::from_data(2048);
    // }

    // if(bn::keypad::l_held())
    // {
    //     _camera.phi -= 4;

    //     if(_camera.phi < 0)
    //     {
    //         _camera.phi += 2048;
    //     }
    // }
    // else if(bn::keypad::r_held())
    // {
    //     _camera.phi += 4;

    //     if(_camera.phi >= 2048)
    //     {
    //         _camera.phi -= 2048;
    //     }
    // }

    _camera.cos = bn::lut_cos(_camera.phi).data() >> 4;
    _camera.sin = bn::lut_sin(_camera.phi).data() >> 4;
    _camera.x += (dir_x * _camera.cos) - (dir_z * _camera.sin);
    _camera.z += (dir_x * _camera.sin) + (dir_z * _camera.cos);
}

void fireyrage_scene::update_hbe_values()
{
    int camera_x = _camera.x.data();
    int camera_y = _camera.y.data() >> 4;
    int camera_z = _camera.z.data();
    int camera_cos = _camera.cos;
    int camera_sin = _camera.sin;
    int y_shift = 160;

    for(int index = 32; index < bn::display::height(); ++index)
    {
        int reciprocal = bn::reciprocal_lut[index].data() >> 4;
        int lam = camera_y * reciprocal >> 12;
        int lcf = lam * camera_cos >> 8;
        int lsf = lam * camera_sin >> 8;

        _pa_values[index] = int16_t(lcf >> 4);
        _pc_values[index] = int16_t(lsf >> 4);

        int lxr = (bn::display::width() / 2) * lcf;
        int lyr = y_shift * lsf;
        _dx_values[index] = (camera_x - lxr + lyr) >> 4;

        lxr = (bn::display::width() / 2) * lsf;
        lyr = y_shift * lcf;
        _dy_values[index] = (camera_z - lxr - lyr) >> 4;
    }
}

fireyrage_scene::fireyrage_scene() : 
    _bg(bn::affine_bg_items::ground.create_bg(-376, -336)),
    _pa_hbe(bn::affine_bg_pa_register_hbe_ptr::create(_bg, _pa_values)),
    _pc_hbe(bn::affine_bg_pc_register_hbe_ptr::create(_bg, _pc_values)),
    _dx_hbe(bn::affine_bg_dx_register_hbe_ptr::create(_bg, _dx_values)),
    _dy_hbe(bn::affine_bg_dy_register_hbe_ptr::create(_bg, _dy_values))
{

}

bn::optional<scene_type> fireyrage_scene::update(){
    bn::optional<scene_type> result;

    update_camera();
    update_hbe_values();
    _pa_hbe.reload_values_ref();
    _pc_hbe.reload_values_ref();
    _dx_hbe.reload_values_ref();
    _dy_hbe.reload_values_ref();
    
    return result;
}

}