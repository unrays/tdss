// Copyright (c) May 2026 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#pragma once

#include "linear_table.hpp"
#include <tuple>
#include <type_traits>

template<typename, template<typename...> typename>
struct make_registry_storage;

template<typename... Entries, template<typename...> typename StorageType>
struct make_registry_storage<LinearTable<Entries...>, StorageType> {
    using type = std::tuple<
        StorageType<typename entry_traits<Entries>::value>...
    >;
};

template<typename Table, template<typename...> typename StorageType>
using make_registry_storage_t = typename make_registry_storage<Table, StorageType>::type;
