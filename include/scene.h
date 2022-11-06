/*
 * scene.h
 * Patterned after fr_scene.h (2020-2022 Gustavo Valiente, gustavo.valiente@protonmail.com). Used under zlib License.
 */

#pragma once

#include "bn_optional.h"

namespace trog {
    enum class scene_type;

    class scene {

        public:
            virtual ~scene() = default;

            [[nodiscard]] virtual bn::optional<scene_type> update() = 0;

        protected:
            scene() = default;
    };

}