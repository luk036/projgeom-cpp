/** @file BM_projgeom.cpp
 *  @brief Google Benchmark suite for projective geometry operations.
 */

#include <benchmark/benchmark.h>

#include <array>
#include <cstdint>

#include <projgeom/ell_object.hpp>
#include <projgeom/hyp_object.hpp>
#include <projgeom/pg_object.hpp>
#include <projgeom/pg_plane.hpp>

// ---------------------------------------------------------------------------
// Dot product
// ---------------------------------------------------------------------------
static void BM_DotProduct(benchmark::State& state) {
    std::array<int64_t, 3> a{1, 2, 3}, b{4, 5, 6};
    for (auto _ : state) {
        auto r = dot(a, b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_DotProduct);

// ---------------------------------------------------------------------------
// Cross product
// ---------------------------------------------------------------------------
static void BM_CrossProduct(benchmark::State& state) {
    std::array<int64_t, 3> a{1, 2, 3}, b{4, 5, 6};
    for (auto _ : state) {
        auto r = cross(a, b);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_CrossProduct);

// ---------------------------------------------------------------------------
// Point creation
// ---------------------------------------------------------------------------
static void BM_PointCreationPg(benchmark::State& state) {
    for (auto _ : state) {
        auto p = PgPoint{{1, 2, 3}};
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_PointCreationPg);

static void BM_PointCreationElliptic(benchmark::State& state) {
    for (auto _ : state) {
        auto p = EllipticPoint{{1, 2, 3}};
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_PointCreationElliptic);

static void BM_PointCreationHyperbolic(benchmark::State& state) {
    for (auto _ : state) {
        auto p = HyperbolicPoint{{1, 2, 3}};
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_PointCreationHyperbolic);

// ---------------------------------------------------------------------------
// Meet (join) operation
// ---------------------------------------------------------------------------
static void BM_MeetPoints(benchmark::State& state) {
    auto p1 = PgPoint{{1, 2, 3}};
    auto p2 = PgPoint{{4, 5, 6}};
    for (auto _ : state) {
        auto l = p1.meet(p2);
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_MeetPoints);

static void BM_MeetLines(benchmark::State& state) {
    auto l1 = PgLine{{1, 0, 0}};
    auto l2 = PgLine{{0, 1, 0}};
    for (auto _ : state) {
        auto p = l1.meet(l2);
        benchmark::DoNotOptimize(p);
    }
}
BENCHMARK(BM_MeetLines);

// ---------------------------------------------------------------------------
// Incident check
// ---------------------------------------------------------------------------
static void BM_Incident(benchmark::State& state) {
    auto p = PgPoint{{1, 2, 3}};
    auto l = PgLine{{4, 5, 6}};
    for (auto _ : state) {
        auto r = p.incident(l);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_Incident);

// ---------------------------------------------------------------------------
// Parametrize
// ---------------------------------------------------------------------------
static void BM_Parametrize(benchmark::State& state) {
    auto p1 = PgPoint{{1, 2, 3}};
    auto p2 = PgPoint{{4, 5, 6}};
    for (auto _ : state) {
        auto r = PgPoint::parametrize(2, p1, 3, p2);
        benchmark::DoNotOptimize(r);
    }
}
BENCHMARK(BM_Parametrize);

// ---------------------------------------------------------------------------
// Perp (pole/polar) in different geometries
// ---------------------------------------------------------------------------
static void BM_PerpElliptic(benchmark::State& state) {
    auto p = EllipticPoint{{1, 2, 3}};
    for (auto _ : state) {
        auto l = p.perp();
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_PerpElliptic);

static void BM_PerpHyperbolic(benchmark::State& state) {
    auto p = HyperbolicPoint{{1, 2, 3}};
    for (auto _ : state) {
        auto l = p.perp();
        benchmark::DoNotOptimize(l);
    }
}
BENCHMARK(BM_PerpHyperbolic);

// ---------------------------------------------------------------------------
// Harmonic conjugate
// ---------------------------------------------------------------------------
static void BM_HarmonicConj(benchmark::State& state) {
    auto a = PgPoint{{1, 0, 1}};
    auto b = PgPoint{{0, 0, 1}};
    auto c = PgPoint{{2, 0, 1}};
    for (auto _ : state) {
        auto d = fun::harm_conj<int64_t>(a, b, c);
        benchmark::DoNotOptimize(d);
    }
}
BENCHMARK(BM_HarmonicConj);

BENCHMARK_MAIN();
