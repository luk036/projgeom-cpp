#pragma once

#include <array>

#include "pg_concepts.hpp"

namespace fun {
/**
 * @brief C-K plane Concept
 *
 * @tparam P Point
 * @tparam L Line
 */
template <class P, class L = typename P::Dual>
concept CKPlanePrim =   //
    ProjPlanePrim<P, L> //
    && requires(const P &p, const L &l) {
  { p.perp() } -> concepts::convertible_to<L>;
};

/**
 * @brief C-K plane Concept (full)
 *
 * @tparam P Point
 * @tparam L Line
 */
template <class P, class L = typename P::Dual>
concept CKPlanePrimDual = CKPlanePrim<P, L> && CKPlanePrim<L, P>;

/**
 * @brief C-K plane Concept
 *
 * @tparam P Point
 * @tparam L Line
 */
template <class V, class P, class L = typename P::Dual>
concept CKPlane =      //
    ProjPlane<V, P, L> //
    && requires(const P &p, const L &l) {
  { p.perp() } -> concepts::convertible_to<L>;
};

/**
 * @brief C-K plane Concept (full)
 *
 * @tparam P Point
 * @tparam L Line
 */
template <class V, class P, class L = typename P::Dual>
concept CKPlaneDual = CKPlane<V, P, L> && CKPlane<V, L, P>;

} // namespace fun
