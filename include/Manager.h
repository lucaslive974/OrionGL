#pragma once

#include <concepts>
#include <memory>
#include <sstream>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace oriongl::core::storage {

template <typename T>
concept streamable_type = requires(std::ostream &os, const T &val) {
  { os << val } -> std::same_as<std::ostream &>;
};

template <typename> struct is_streamable_vector : std::false_type {};

template <typename T, typename Alloc>
struct is_streamable_vector<std::vector<T, Alloc>>
    : std::bool_constant<streamable_type<T>> {};

template <typename T>
concept streamable_vector = is_streamable_vector<std::remove_cvref_t<T>>::value;

template <typename T>
concept streamable = streamable_type<T> || streamable_vector<T>;

using Key = std::string;

template <typename T> class Manager {
  using cache_map = std::unordered_map<Key, std::weak_ptr<T>>;
  cache_map map_;

public:
  std::shared_ptr<T> tryToLockSmartPointer(Key &key, cache_map &map) {
    auto it = map.find(key);
    if (it == map.end())
      return nullptr;

    return map[key].lock();
  };

  template <typename... Args>
  std::shared_ptr<T> instanciateAndCache(Key &key, cache_map &map,
                                         Args &&...args) {
    auto _ptr = std::make_shared<T>(std::forward<Args>(args)...);
    map[key] = _ptr;
    return _ptr;
  };

  template <streamable... Args>
  std::string concatenateHashKeys(Args &&...args) {
    std::stringstream ss;

    auto append = [&]<typename V>(V &&value) {
      if constexpr (streamable_vector<V>) {
        for (const auto &e : value)
          ss << e;
      } else {
        ss << value;
      }
    };

    (append(std::forward<Args>(args)), ...);

    return ss.str();
  }

  template <typename... Args>
    requires std::constructible_from<T, Args...>
  std::shared_ptr<T> createResource(Args... args) {
    auto key = concatenateHashKeys(args...);
    auto ptr = tryToLockSmartPointer(key, map_);

    if (ptr)
      return ptr;

    return instanciateAndCache(key, map_, args...);
  };

  template <typename K, typename... Args>
    requires(!std::constructible_from<T, K, Args...>)
  std::shared_ptr<T> createResource(K key, Args... args) {
    std::string key_{key};
    auto ptr = tryToLockSmartPointer(key_, map_);

    if (ptr)
      return ptr;

    return instanciateAndCache(key_, map_, args...);
  }
};
}; // namespace oriongl::core::storage
