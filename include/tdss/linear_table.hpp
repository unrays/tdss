// Copyright (c) May 2026 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#pragma once
#include <cstddef>

/***************************************************************************/

struct sentinel_t {};

/***************************************************************************/

template<typename... Types>
struct LinearTable : Types... {};

template<typename Key, typename Value>
struct Entry {};

/***************************************************************************/

template<typename...>
struct entry_traits;

template<typename K, typename V>
struct entry_traits<Entry<K, V>> { using key = K; using value = V; };

/***************************************************************************/

template<typename...>
struct lookup;

template<typename Key, typename Target, typename... Rest>
struct lookup<Key, LinearTable<Entry<Key, Target>, Rest...>> {
    using type = Target;
};

template<typename Key, typename Head, typename... Rest>
struct lookup<Key, LinearTable<Head, Rest...>> {
    using type = typename lookup<Key, LinearTable<Rest...>>::type;
};

template<typename Key, typename Empty>
struct lookup<Key, Empty> {
    using type = sentinel_t;
};

template<typename Key, typename Table>
using table_lookup_t = typename lookup<Key, Table>::type;

/***************************************************************************/

template<typename>
struct size_of;

template<typename... Types>
struct size_of<LinearTable<Types...>> {
    static constexpr std::size_t value = sizeof...(Types);
};

template<typename Table>
static constexpr std::size_t size_of_v = size_of<Table>::value;

/***************************************************************************/
