/*
 * scene.h
 * Patterned after fr_scene.h (2020-2022 Gustavo Valiente, gustavo.valiente@protonmail.com). Used under zlib License.
 */

#pragma once

#include <bn_optional.h>
#include <bn_regular_bg_ptr.h>
#include "enums.h"
#include "constants.h"

namespace trog {

    class scene {

        public:
            virtual ~scene() = default;

            [[nodiscard]] virtual bn::optional<scene_type> update() = 0;

        protected:
            scene() = default;
            void move_background(bn::regular_bg_ptr bg);
    };

}
