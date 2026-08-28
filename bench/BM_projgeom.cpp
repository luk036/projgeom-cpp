/** @file BM_projgeom.cpp
 *  @brief nanobench suite for projective geometry operations.
 */

#define ANKERL_NANOBENCH_IMPLEMENT
#include <nanobench.h>

#include <array>
#include <cstdint>
#include <projgeom/ell_object.hpp>
#include <projgeom/hyp_object.hpp>
#include <projgeom/pg_object.hpp>
#include <projgeom/pg_plane.hpp>

int main() {
    {
        ankerl::nanobench::Bench bench;
        bench.title("Projective geometry operations")
            .unit("op")
            .warmup(100)
            .epochs(50)
            .minEpochIterations(10000);

        std::array<int64_t, 3> a{1, 2, 3}, b{4, 5, 6};
        bench.run("DotProduct", [&] {
            auto r = dot(a, b);
            ankerl::nanobench::doNotOptimizeAway(r);
        });

        bench.run("CrossProduct", [&] {
            auto r = cross(a, b);
            ankerl::nanobench::doNotOptimizeAway(r);
        });

        bench.run("PointCreationPg", [&] {
            auto p = PgPoint{{1, 2, 3}};
            ankerl::nanobench::doNotOptimizeAway(p);
        });

        bench.run("PointCreationElliptic", [&] {
            auto p = EllipticPoint{{1, 2, 3}};
            ankerl::nanobench::doNotOptimizeAway(p);
        });

        bench.run("PointCreationHyperbolic", [&] {
            auto p = HyperbolicPoint{{1, 2, 3}};
            ankerl::nanobench::doNotOptimizeAway(p);
        });

        auto p1 = PgPoint{{1, 2, 3}};
        auto p2 = PgPoint{{4, 5, 6}};
        bench.run("MeetPoints", [&] {
            auto l = p1.meet(p2);
            ankerl::nanobench::doNotOptimizeAway(l);
        });

        auto l1 = PgLine{{1, 0, 0}};
        auto l2 = PgLine{{0, 1, 0}};
        bench.run("MeetLines", [&] {
            auto p = l1.meet(l2);
            ankerl::nanobench::doNotOptimizeAway(p);
        });

        auto p = PgPoint{{1, 2, 3}};
        auto l = PgLine{{4, 5, 6}};
        bench.run("Incident", [&] {
            auto r = p.incident(l);
            ankerl::nanobench::doNotOptimizeAway(r);
        });

        bench.run("Parametrize", [&] {
            auto r = PgPoint::parametrize(2, p1, 3, p2);
            ankerl::nanobench::doNotOptimizeAway(r);
        });
    }

    {
        ankerl::nanobench::Bench bench;
        bench.title("Perp (pole/polar) and harmonic conjugate")
            .unit("op")
            .warmup(100)
            .epochs(50)
            .minEpochIterations(10000);

        auto pe = EllipticPoint{{1, 2, 3}};
        bench.run("PerpElliptic", [&] {
            auto l = pe.perp();
            ankerl::nanobench::doNotOptimizeAway(l);
        });

        auto ph = HyperbolicPoint{{1, 2, 3}};
        bench.run("PerpHyperbolic", [&] {
            auto l = ph.perp();
            ankerl::nanobench::doNotOptimizeAway(l);
        });

        auto a = PgPoint{{1, 0, 1}};
        auto b = PgPoint{{0, 0, 1}};
        auto c = PgPoint{{2, 0, 1}};
        bench.run("HarmonicConj", [&] {
            auto d = fun::harm_conj<int64_t>(a, b, c);
            ankerl::nanobench::doNotOptimizeAway(d);
        });
    }
}
