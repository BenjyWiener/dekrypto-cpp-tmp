#pragma once

#include <numeric>
#include <type_traits>

template <int N, int D = 1>
struct rational {
    static inline constexpr int num = N / std::gcd(N, D);
    static inline constexpr int denom = D / std::gcd(N, D);
    static inline constexpr double as_double = (double)num / (double)denom;
};

enum operation {
    add,
    subtract,
    multiply,
    divide
};

// TODO: Use concepts
template <operation, typename, typename>
struct apply_op;

template <typename Lhs, typename Rhs>
struct apply_op<add, Lhs, Rhs> {
    using value = rational<(Lhs::num * Rhs::denom) + (Rhs::num * Lhs::denom), Lhs::denom * Rhs::denom>;
};

template <typename Lhs, typename Rhs>
struct apply_op<multiply, Lhs, Rhs> {
    using value = rational<Lhs::num * Rhs::num, Lhs::denom * Rhs::denom>;
};

template <typename Lhs, typename Rhs>
struct apply_op<subtract, Lhs, Rhs> {
    using value = rational<(Lhs::num * Rhs::denom) - (Rhs::num * Lhs::denom), Lhs::denom * Rhs::denom>;
};

template <typename Lhs, typename Rhs>
struct apply_op<divide, Lhs, Rhs> {
    using value = rational<Lhs::num * Rhs::denom, Lhs::denom * Rhs::num>;
};

enum class expression_type {
    term,
    expression
};

template <expression_type, auto, typename...>
struct expression;

template <int V>
struct expression<expression_type::term, V> {
    static inline constexpr int value = V;
    using evaluated = rational<V>;
};

template <int V>
using term = expression<expression_type::term, V>;

template <operation Op, typename LHS, typename RHS>
struct expression<expression_type::expression, Op, LHS, RHS> {
    static inline constexpr operation op = Op;
    using lhs = LHS;
    using rhs = RHS;
    using evaluated = apply_op<op, typename lhs::evaluated, typename rhs::evaluated>::value;
};

template <operation Op, typename LHS, typename RHS>
using expr = expression<expression_type::expression, Op, LHS, RHS>;

template <typename T>
struct is_expr : std::false_type {};

template <operation Op, typename LHS, typename RHS>
struct is_expr<expr<Op, LHS, RHS>> : std::true_type {};

template <int V>
struct is_expr<term<V>> : std::true_type {};

template <typename T>
struct is_term : std::false_type {};

template <int V>
struct is_term<term<V>> : std::true_type {};

template <typename T>
concept Expr = is_expr<T>::value;

template <typename T>
concept Term = Expr<T> &&is_term<T>::value;
