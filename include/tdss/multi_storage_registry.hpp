// Copyright (c) May 2026 Félix-Olivier Dumas. All rights reserved.
// Licensed under the terms described in the LICENSE file

#pragma once

#include "linear_table.hpp"
#include "smart_storage.hpp"
#include "storage_traits.hpp"
#include <cstddef>
#include <tuple>
#include <utility>
#include <type_traits>
#include <iostream>

/***************************************************************************/

template<typename Table, typename HandleProvider, std::size_t N = 1 << 14>
class MultiStorageRegistry final {
protected:
    static constexpr std::size_t BytesPerStorage = N / size_of_v<Table>;

    template<typename Up> using RegistryStorageStrategy = SmartStorage<Up, BytesPerStorage / sizeof(Up)>;

public:
    explicit MultiStorageRegistry(HandleProvider provider = {})
        : handleProvider_(provider)
        , storage_{}
    {}

    ~MultiStorageRegistry() noexcept { reset(); }

public:
    template<typename Tp>
    auto& resolve_storage() noexcept
    {
        using Result = std::decay_t<
            table_lookup_t<std::decay_t<Tp>, Table>
        >;

        static_assert(
            !std::is_same_v<Result, sentinel_t>,
            "Unable to resolve the requested type from the LinearTable."
        );

        return std::get<RegistryStorageStrategy<Result>>(storage_);
    }

public:
    template<typename Tp>
    PRYSMA_NODISCARD const auto& get(const Tp* obj) const noexcept
    {
        const auto& storage = resolve_storage<Tp>();
        return storage.get(handleProvider_(obj));
    }

    template<typename Up, typename Tp>
    PRYSMA_NODISCARD const auto& get_for(const Tp* obj) const noexcept
    {
        const auto& storage = std::get<RegistryStorageStrategy<Up>>(storage_);
        return storage.get(handleProvider_(obj));
    }

public:
    template<typename Tp>
    PRYSMA_NODISCARD auto& get(const Tp* obj) noexcept
    {
        auto& storage = resolve_storage<Tp>();
        return storage.get(handleProvider_(obj));
    }

    template<typename Up, typename Tp>
    PRYSMA_NODISCARD auto& get_for(const Tp* obj) noexcept
    {
        auto& storage = std::get<RegistryStorageStrategy<Up>>(storage_);
        return storage.get(handleProvider_(obj));
   }

public:
    template<typename Tp, typename... Types>
    auto& construct(const Tp* obj, Types&&... args)
    {
        auto& storage = resolve_storage<Tp>();
        return storage.emplace(handleProvider_(obj), std::forward<Types>(args)...);
    }

    template<typename Up, typename Tp, typename... Types>
    auto& construct_for(const Tp* obj, Types&&... args)
    {
        auto& storage = std::get<RegistryStorageStrategy<Up>>(storage_);
        return storage.emplace(handleProvider_(obj), std::forward<Types>(args)...);
    }

public:
    template<typename Tp, typename Up>
    auto& assign(const Tp* obj, Up&& arg)
    {
        auto& storage = resolve_storage<Tp>();
        return storage.insert(handleProvider_(obj), std::forward<Up>(arg));
    }

    template<typename Up, typename Tp>
    auto& assign_for(const Tp* obj, Up&& arg)
    {
        auto& storage = std::get<RegistryStorageStrategy<Up>>(storage_);
        return storage.insert(handleProvider_(obj), std::forward<Up>(arg));
    }

public:
    void reset() noexcept
    {
        std::apply([](auto&... storage) {
            (storage.reset(), ...);
        }, storage_);
    }

    template<typename Up>
    void reset_for() noexcept
    {
        auto& storage = std::get<RegistryStorageStrategy<Up>>(storage_);
        storage.reset();
    }

private:
    make_registry_storage_t<Table, RegistryStorageStrategy> storage_;
    HandleProvider handleProvider_;
};

/***************************************************************************/
