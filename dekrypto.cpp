#include <iostream>

#include "debug.h"
#include "expression.h"
#include "list.h"

using namespace tmp_list;

template <typename T1, typename T2>
struct pair {
    using first = T1;
    using second = T2;
};

template <List R, List L = void>
struct halves {
    using value =
        list<pair<typename L::append<typename R::value>, typename R::next>,
             typename halves<typename R::next,
                             typename L::append<typename R::value>>::value>;
};

template <List R>
struct halves<R, void> {
    using value =
        list<pair<list<typename R::value>, typename R::next>,
             typename halves<typename R::next, list<typename R::value>>::value>;
};

template <ListStub R, List L>
struct halves<R, L> {
    using value = void;
};

template <typename First, List L>
struct make_pairs {
    using value = list<pair<First, typename L::value>,
                       typename make_pairs<First, typename L::next>::value>;
};

template <typename First, ListStub L>
struct make_pairs<First, L> {
    using value = list<pair<First, typename L::value>>;
};

template <List L1, List L2>
struct combinations {
    using value = typename make_pairs<typename L1::value, L2>::value::concat<
        typename combinations<typename L1::next, L2>::value>;
};

template <ListStub L1, List L2>
struct combinations<L1, L2> {
    using value = make_pairs<typename L1::value, L2>::value;
};

template <List L>
struct insert_ops {
    using value = typename make_list<
        expr<add, typename L::value::first, typename L::value::second>,
        expr<subtract, typename L::value::first, typename L::value::second>,
        expr<multiply, typename L::value::first, typename L::value::second>,
        expr<divide, typename L::value::first, typename L::value::second>>::
        concat<typename insert_ops<typename L::next>::value>;
};

template <ListStub L>
struct insert_ops<L> {
    using value = make_list<
        expr<add, typename L::value::first, typename L::value::second>,
        expr<subtract, typename L::value::first, typename L::value::second>,
        expr<multiply, typename L::value::first, typename L::value::second>,
        expr<divide, typename L::value::first, typename L::value::second>>;
};

template <ListNT<int> L>
struct expressions {
   private:
    template <typename P>
    struct lambda {
        using value = insert_ops<typename combinations<
            typename expressions<typename P::first>::value,
            typename expressions<typename P::second>::value>::value>::value;
    };

   public:
    using value =
        flatten<typename map<typename halves<L>::value, lambda>::value>::value;
};

template <ListNT<int> L>
requires ListStub<L> struct expressions<L> {
    using value = list<term<L::value::value>>;
};

template <ListNT<int> Cards, int Goal>
struct solve {
   private:
    template <Expr E>
    using is_solution = std::bool_constant<E::evaluated::as_double == Goal>;
   public:
    using value = flatten<map<permute<make_list_nt<1, 2, 3>>::value, expressions>::value>::value::filter<is_solution>;
};

int main() {
    // std::cout << apply_op<add, rational<2,5>,
    // rational<1,100>>::value::as_double << std::endl;

    // std::cout << expr<add, expr<multiply, term<2>, term<6>>,
    // term<7>>::evaluated::as_double << std::endl; std::cout << expr<divide,
    // term<2>, term<10>>::evaluated::as_double << std::endl;

    // print_all< make_list<1, 2, 3, 4, 5, 6>::head<2> >();
    // print_all< make_list<1, 2, 3, 4, 5, 6>::tail<4> >();
    // print_all< make_list_nt<1, 2, 3, 4, 5, 6>::tail<2>::head<2> >();
    // print_all< sublist_to<make_list<1>, 1>::value >();
    // print_all< make_list_nt<1, 2>::concat<make_list_nt<9, 8>> >();

    // std::cout << make_list_nt<1, 2, 7>::last::value << std::endl;
    // std::cout << get_last<make_list_nt<1, 2, 3>>::value::value << std::endl;

    // print_all_2d< interleave<make_list_nt<1, 2, 3>, std::integral_constant<int,
    // 0>>::value >(); print_all< interleave<make_list_nt<1>,
    // std::integral_constant<int, 0>>::value::value >();

    // print_all<map<make_list_nt<1, 2, 3, 4>, times_two>::value>();

    // print_all< flatten<interleave<make_list_nt<1, 2, 3>,
    // std::integral_constant<int, 0>>::value>::value >(); print_all<
    // flatten<make_list<make_list_nt<1, 2, 3>>>::value >(); print_all<
    // flatten<make_list<make_list_nt<1, 2, 3>, make_list_nt<4, 5, 6>,
    // make_list_nt<7, 8, 9>>>::value >(); print_all_2d< make_list<make_list_nt<1,
    // 2, 3>, make_list_nt<4, 5, 6>> >();

    // print_all_2d< permute<make_list_nt<1, 2, 3, 4, 5>>::value >();

    // std::cout << make_list_nt<1,2,3,4,5,6,7,8,9,10>::length << std::endl;

    // std::cout << make_list_nt<1,2,3,4,5,6,7,8,9,10>::get<5>::value <<
    // std::endl;

    // print_all_pairs< halves<make_list_nt<1, 2, 3, 4, 5>>::value >();

    // print_all_pairs< flatten<map<permute<make_list_nt<1, 2, 3, 4>>::value,
    // halves>::value>::value >();

    // std::cout << flatten<map<permute<make_list_nt<1, 2, 3, 4>>::value,
    // halves>::value>::value::length << std::endl;

    // print_all_pairs< map<make_list<make_list_nt<1>, make_list_nt<2>,
    // make_list_nt<3>>, pair_creator<make_list_nt<6>>::creator>::value >();
    // print_all_pairs< make_pairs<make_list_nt<'A'>, make_list<make_list_nt<1>,
    // make_list_nt<2>, make_list_nt<3>>>::value >();

    // print_all_pairs< combinations<make_list<make_list_nt<1>, make_list_nt<2>,
    // make_list_nt<3>>,  make_list<make_list_nt<'a'>, make_list_nt<'b'>,
    // make_list_nt<'c'>>>::value >(); print_all_pairs<
    // combinations<make_list<make_list_nt<1>, make_list_nt<2>>,
    // make_list<make_list_nt<'a'>>>::value >();

    // print_expr< expr<add, expr<multiply, term<2>, term<6>>, term<7>> >();
    // print_expr< term<99> >();
    // print_expr< expr<add, term<2>, term<5>> >();

    // print_all_exprs< flatten<map<permute<make_list_nt<1, 2, 3>>::value, expressions>::value>::value >();

    // print_all< make_list_nt<1, 2, 3>::append<void>::append<std::integral_constant<int, 37>>::filter<std::is_compound_v> >();

    // print_all_exprs< solve<make_list_nt<1, 2, 3>, 2>::value >();

    print_all_exprs< solve<make_list_nt<
    #include "input/cards.txt"
    >,
    #include "input/goal.txt"
    >::value >();

    return 0;
}