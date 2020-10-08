#include <iostream>

#include "expression.h"
#include "list.h"

using namespace tmp_list;

template <List L>
void print_all() {
    std::cout << L::value::value << ' ';
    print_all<typename L::next>();
}

template <>
void print_all<void>() {
    std::cout << std::endl;
};

template <List L>
void print_all_2d() {
    print_all<typename L::value>();
    std::cout << std::endl;
    print_all_2d<typename L::next>();
}

template <>
void print_all_2d<void>(){};

template <List L>
void print_all_pairs() {
    print_all<typename L::value::first>();
    print_all<typename L::value::second>();
    std::cout << std::endl;
    print_all_pairs<typename L::next>();
}

template <>
void print_all_pairs<void>(){};

template <operation Op>
struct show_op_impl;

template <>
struct show_op_impl<add> {
    static inline constexpr const char *value = " + ";
};

template <>
struct show_op_impl<subtract> {
    static inline constexpr const char *value = " - ";
};

template <>
struct show_op_impl<multiply> {
    static inline constexpr const char *value = " * ";
};

template <>
struct show_op_impl<divide> {
    static inline constexpr const char *value = " / ";
};

template <operation Op>
inline constexpr const char *show_op = show_op_impl<Op>::value;

template <Expr E>
struct print_expr_impl {
    static void f() {
        std::cout << '(';
        print_expr_impl<typename E::lhs>::f();
        std::cout << show_op<E::op>;
        print_expr_impl<typename E::rhs>::f();
        std::cout << ')';
    }
};

template <Term T>
struct print_expr_impl<T> {
    static void f() { std::cout << T::value; }
};

template <Expr E>
void print_expr() {
    print_expr_impl<E>::f();
    std::cout << " = " << E::evaluated::as_double << std::endl;
};

template <List L>
void print_all_exprs() {
    print_expr<typename L::value>();
    print_all_exprs<typename L::next>();
}

template <>
void print_all_exprs<void>() {
    std::cout << std::endl;
};
