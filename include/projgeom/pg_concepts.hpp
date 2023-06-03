#pragma once

#include <array>
#include <cassert>

#include "common_concepts.h"

namespace fun {
/**
 * @brief Projective plane Concept
 *
 * @tparam P Point
 * @tparam L Line
 */
template <class P, class L>
concept ProjPlanePrim =              //
    concepts::equality_comparable<P> //
    && requires(const P &p, const P &q, const L &l) {
         { p.incident(l) } -> concepts::convertible_to<bool>; // incidence
         { p.circ(q) } -> concepts::convertible_to<L>;        // join or meet
       };

/**
 * @brief Projective plane Concept (full)
 *
 * @tparam P Point
 * @tparam L Line
 */
template <class P, class L>
concept ProjPlanePrimDual = ProjPlanePrim<P, L> && ProjPlanePrim<L, P>;

/**
 * @brief Projective plane Concept
 *
 * @tparam V
 * @tparam P Point
 * @tparam L Line
 */
template <typename V, class P, class L>
concept ProjPlane =
    concepts::equality_comparable<P> && ProjPlanePrim<P, L> //
    && requires(const P &p, const P &q, const L &l, const V &a) {
         { p.aux() } -> concepts::convertible_to<L>; // line not incident with p
         { p.dot(l) } -> concepts::convertible_to<V>; // for basic measurement
         { P::plucker(a, p, a, q) } -> concepts::convertible_to<P>;
       };

/**
 * @brief Projective plane dual Concept
 *
 * @tparam V
 * @tparam P Point
 * @tparam L Line
 */
template <typename V, class P, class L>
concept ProjPlaneDual = ProjPlane<V, P, L> && ProjPlane<V, L, P>;

} // namespace fun
