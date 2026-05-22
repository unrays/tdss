// Copyright (c) May 2026 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#pragma once

#include <cstddef>
#include <iostream>

struct DefaultHandleProvider {
    template<typename Tp>
    [[nodiscard]] constexpr std::size_t operator()(const Tp* obj) {
        return obj->id;
    }
};
