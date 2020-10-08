#pragma once

#include <type_traits>

namespace tmp_list {
template <typename V, typename Next>
struct list;

template <typename T>
struct is_list : std::false_type {};

template <typename V, typename Next>
struct is_list<list<V, Next>> : is_list<Next> {};

template <typename V>
struct is_list<list<V, void>> : std::true_type {};

template <>
struct is_list<void> : std::true_type {};

template <typename T>
concept List = is_list<T>::value;

// List of `integral_constant`s with `value_type` == `ItemT`
template <typename T, typename ItemT>
concept ListNT = List<T>&& std::is_same_v<typename T::value::value_type, ItemT>;

template <typename T>
concept ListStub = List<T>&& std::is_void_v<typename T::next>;

template <List L, int N>
struct sublist_to;

template <List L, int N>
struct sublist_from;

template <List L1, List L2>
struct concatenate_lists;

template <List L>
struct get_last;

template <List L>
struct get_length;

template <List L, int I>
struct get_index;

template <List L, template <typename> class Predicate>
struct filter;

template <typename V, typename Next = void>
struct list {
    using value = V;
    using next = Next;
    using self = list<value, next>;

    template <int N>
    using take = sublist_to<self, N>::value;

    template <int N>
    using drop = sublist_from<self, N>::value;

    template <List L>
    using concat = concatenate_lists<self, L>::value;

    template <typename NewV>
    using append = concat<list<NewV>>;

    template <typename NewV>
    using prepend = list<NewV, self>;

    using last = std::conditional_t<std::is_void_v<Next>, V,
                                    typename get_last<Next>::value>;

    static inline constexpr int length = get_length<self>::value;

    template <int I>
    using get = get_index<self, I>::value;

    template <template <typename> class Predicate>
    using filter = filter<self, Predicate>::value;
};

template <typename... Vs>
struct make_list_impl;

template <typename V, typename... Vs>
struct make_list_impl<V, Vs...> {
    using value = list<V, typename make_list_impl<Vs...>::value>;
};

template <typename V>
struct make_list_impl<V> {
    using value = list<V>;
};

template <typename... Vs>
using make_list = make_list_impl<Vs...>::value;

template <auto... Vs>
using make_list_nt = make_list<std::integral_constant<decltype(Vs), Vs>...>;

template <List L, int N>
struct sublist_to {
    using value = list<typename L::value,
                       typename sublist_to<typename L::next, N - 1>::value>;
};

template <List L>
struct sublist_to<L, 0> {
    using value = void;
};

template <List L, int N>
struct sublist_from {
    using value = sublist_from<typename L::next, N - 1>::value;
};

template <List L>
struct sublist_from<L, 0> {
    using value = L;
};

template <List L1, List L2>
struct concatenate_lists {
    using value = list<typename L1::value,
                       typename concatenate_lists<typename L1::next, L2>::value>;
};

template <List L2>
struct concatenate_lists<void, L2> {
    using value = L2;
};

template <List L>
struct get_last {
    using value = get_last<typename L::next>::value;
};

template <typename V>
struct get_last<list<V, void>> {
    using value = V;
};

template <>
struct get_last<void> {
    using value = void;
};

template <List L>
struct get_length {
    static inline constexpr int value = 1 + L::next::length;
};

template <typename V>
struct get_length<list<V>> {
    static inline constexpr int value = 1;
};

template <>
struct get_length<void> {
    static inline constexpr int value = 0;
};

template <List L, int I>
struct get_index {
    using value = get_index<typename L::next, I - 1>::value;
};

template <typename L>
struct get_index<L, 0> {
    using value = L::value;
};

template <List L, template <typename> class Predicate>
struct filter {
    using value = filter<typename L::next, Predicate>::value;
};

template <List L, template <typename> class Predicate>
requires (Predicate<typename L::value>::value && !ListStub<L>) struct filter<L, Predicate> {
    using value = list<typename L::value, typename filter<typename L::next, Predicate>::value>;
};

template <ListStub L, template <typename> class Predicate>
struct filter<L, Predicate> {
    using value = void;
};

template <ListStub L, template <typename> class Predicate>
requires (Predicate<typename L::value>::value) struct filter<L, Predicate> {
    using value = list<typename L::value>;
};

template <List R, typename V, List L = void>
struct interleave {
    using value =
        list<typename L::append<V>::concat<R>,
             typename interleave<typename R::next, V,
                                 typename L::append<typename R::value>>::value>;
};

template <List R, typename V>
struct interleave<R, V, void> {
    using value = list<
        typename R::prepend<V>,
        typename interleave<typename R::next, V, list<typename R::value>>::value>;
};

template <List L, template <typename> class F>
struct map {
    using value = list<typename F<typename L::value>::value,
                       typename map<typename L::next, F>::value>;
};

template <typename V, template <typename> class F>
struct map<list<V>, F> {
    using value = list<typename F<V>::value>;
};

template <List L>
struct flatten {
    using value =
        typename L::value::concat<typename flatten<typename L::next>::value>;
};

template <List L>
struct flatten<list<L>> {
    using value = L;
};

template <typename V, List L>
struct interleave<void, V, L> {
    using value = list<typename L::append<V>>;
};

template <typename V>
struct interleave<void, V> {
    using value = list<list<V>>;
};

template <List L>
struct permute {
   private:
    template <List Perms>
    using interleave_head = interleave<Perms, typename L::value>;

   public:
    using value = flatten<typename map<typename permute<typename L::next>::value,
                                       interleave_head>::value>::value;
};

template <typename V>
struct permute<list<V>> {
    using value = list<list<V>>;
};
}  // namespace tmp_list
