#ifndef SAL_WRAPPER_HPP_INCLUDED_
#define SAL_WRAPPER_HPP_INCLUDED_

// implement here
#include <functional>
#include <optional>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

#include "super-awesome-library-that-has-breaking-changes-frequently.hpp"

namespace sal = super_awesome_library_that_has_breaking_changes_frequently;

namespace sal_wrapper {

namespace detail {

#define SAL_WRAPPER_DETAIL_DEF(name, expr)                               \
  template <class T, class = void>                                       \
  struct name##_impl : ::std::false_type {};                             \
  template <class T>                                                     \
  struct name##_impl<T, ::std::enable_if_t<expr>> : ::std::true_type {}; \
  template <class T>                                                     \
  using name = name##_impl<T>;                                           \
  template <class T>                                                     \
  inline constexpr bool name##_v = name<T>::value;

#define SAL_WRAPPER_DETAIL_UNWRAP(...) __VA_ARGS__
SAL_WRAPPER_DETAIL_DEF(
    has_a, SAL_WRAPPER_DETAIL_UNWRAP(
               ::std::is_same_v<decltype(::std::declval<T>().a(
                                    ::std::declval<::std::size_t>())),
                                int>))
SAL_WRAPPER_DETAIL_DEF(
    has_a2, SAL_WRAPPER_DETAIL_UNWRAP(
                ::std::is_same_v<decltype(::std::declval<T>().a2(
                                     ::std::declval<::std::size_t>())),
                                 int>))
SAL_WRAPPER_DETAIL_DEF(
    has_bp, SAL_WRAPPER_DETAIL_UNWRAP(
                ::std::is_same_v<decltype(::std::declval<T>().b(
                                     ::std::declval<::std::size_t>())),
                                 const int*>))
SAL_WRAPPER_DETAIL_DEF(
    has_bo, SAL_WRAPPER_DETAIL_UNWRAP(
                ::std::is_same_v<decltype(::std::declval<T>().b(
                                     ::std::declval<::std::size_t>())),
                                 ::std::optional<const int*>>))
SAL_WRAPPER_DETAIL_DEF(
    has_c, SAL_WRAPPER_DETAIL_UNWRAP(::std::is_same_v<decltype(T::c()), int>))
SAL_WRAPPER_DETAIL_DEF(
    has_d1,
    SAL_WRAPPER_DETAIL_UNWRAP(
        ::std::is_same_v<decltype(::std::declval<T>().d(::std::declval<int>())),
                         void>))
SAL_WRAPPER_DETAIL_DEF(
    has_d2, SAL_WRAPPER_DETAIL_UNWRAP(
                ::std::is_same_v<decltype(::std::declval<T>().d(
                                     ::std::declval<int>(),
                                     ::std::declval<::sal::super_flags>())),
                                 void>))
SAL_WRAPPER_DETAIL_DEF(
    has_idx,
    SAL_WRAPPER_DETAIL_UNWRAP(
        ::std::conjunction_v<
            ::std::is_same<decltype(::std::declval<T>().hyper_size()),
                           ::std::size_t>,
            ::std::is_same<decltype(::std::declval<T>().access_hyper_data(
                               ::std::declval<::std::size_t>())),
                           const ::std::string&>>))
SAL_WRAPPER_DETAIL_DEF(
    has_rng,
    SAL_WRAPPER_DETAIL_UNWRAP(
        ::std::conjunction_v<
            ::std::is_same<
                decltype(::std::declval<T>().hypers().begin()->access()),
                const ::std::string&>,
            ::std::is_same<
                decltype(::std::declval<T>().hypers().end()->access()),
                const ::std::string&>>))
#undef SAL_WRAPPER_DETAIL_UNWRAP
#undef SAL_WRAPPER_DETAIL_DEF

template <class T, class...>
struct sal_type_identity {
  using type = T;
};
template <class T, class... Args>
using sal_type_identity_t = typename sal_type_identity<T, Args...>::type;

template <class A>
constexpr int a_impl(const A& awesome, ::std::size_t i) {
  static_assert(::std::is_base_of_v<::sal::awesome_type, A>);
  if constexpr (has_a_v<A>) {
    return awesome.a(i);
  } else if constexpr (has_a2_v<A>) {
    return awesome.a2(i);
  } else {
    static_assert(!::std::is_base_of_v<::sal::awesome_type, A>);
    return 0;
  }
}

template <class A>
constexpr const int* b_impl(const A& awesome, ::std::size_t i) {
  static_assert(::std::is_base_of_v<::sal::awesome_type, A>);
  if constexpr (has_bp_v<A>) {
    return awesome.b(i);
  } else if constexpr (has_bo_v<A>) {
    return awesome.b(i).value_or(nullptr);
  } else {
    static_assert(!::std::is_base_of_v<::sal::awesome_type, A>);
    return nullptr;
  }
}

}  // namespace detail

constexpr int a(const ::sal::awesome_type& awesome, ::std::size_t i) {
  return detail::a_impl(awesome, i);
}

constexpr const int* b(const ::sal::awesome_type& awesome, ::std::size_t i) {
  return detail::b_impl(awesome, i);
}

}  // namespace sal_wrapper

namespace super_awesome_library_that_has_breaking_changes_frequently {

namespace detail {

template <class... Args>
constexpr auto c_impl(Args&&... args) {
  using A = ::sal_wrapper::detail::sal_type_identity_t<awesome_type, Args...>;
  if constexpr (::sal_wrapper::detail::has_c_v<A>)
    return A::c(::std::forward<Args>(args)...);
  else
    return c(::std::forward<Args>(args)...);
}

}  // namespace detail

}  // namespace super_awesome_library_that_has_breaking_changes_frequently

namespace sal_wrapper {

template <class... Args>
constexpr int c(Args&&... args) {
  return ::sal::detail::c_impl(::std::forward<Args>(args)...);
}

namespace detail {

template <class E, class = void>
struct excellent_base_impl;
template <class E>
struct excellent_base_impl<E, ::std::enable_if_t<has_d1_v<E>>> : E {};

template <class E>
struct excellent_base_impl<E, ::std::enable_if_t<has_d2_v<E>>> : E {
  virtual void d(int) = 0;
  void d(int l, ::sal::super_flags) { this->d(l); }
};

template <class E>
auto hypers_impl(const E& excellent) {
  if constexpr (has_idx_v<E>) {
    struct hyper_data {
      const E* ex;
      ::std::size_t i;
      hyper_data(const E* e, ::std::size_t i) : ex(e), i(i) {}
      const ::std::string& access() const { return ex->access_hyper_data(i); }
    };
    struct hyper_range {
      const E* ex;
      hyper_range(const E* e) : ex(e) {
        ::std::size_t sz = ex->hyper_size();
        data.reserve(sz);
        for (size_t i = 0; i < sz; ++i) data.emplace_back(ex, i);
      }
      auto begin() const { return data.begin(); }
      auto end() const { return data.end(); }

     private:
      ::std::vector<hyper_data> data;
    };
    return hyper_range(&excellent);
  } else if constexpr (has_rng_v<E>)
    return excellent.hypers();
}

}  // namespace detail

struct excellent_base : detail::excellent_base_impl<::sal::excellent_type> {};

auto hypers(const ::sal::excellent_type& excellent) {
  return detail::hypers_impl(excellent);
}

}  // namespace sal_wrapper

#endif  // SAL_WRAPPER_HPP_INCLUDED_

