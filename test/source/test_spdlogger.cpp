#include <doctest/doctest.h>
#include <projgeom/logger.hpp>
#include <projgeom/persp_object.hpp>
#include <iostream>
#include <fstream>

TEST_CASE("Spdlogger basic test") {
    std::cout << "=== Testing spdlogger integration ===" << std::endl;
    
    // Test wrapper function
    projgeom::log_with_spdlog("Spdlogger test initialization");
    
    // Test with geometric operations
    auto pt_p = PerspPoint({3, 4, 5});
    auto pt_q = PerspPoint({0, 4, 1});
    auto ln_m = PerspLine({1, 0, 4});
    
    bool is_ok = fun::check_axiom(pt_p, pt_q, ln_m);
    projgeom::log_with_spdlog(is_ok ? "Axiom check passed" : "Axiom check failed");
    
    CHECK(is_ok);
    
    auto t = fun::altitude(pt_p, ln_m);
    bool res = fun::is_perpendicular(t, ln_m);
    projgeom::log_with_spdlog(res ? "Perpendicular check passed" : "Perpendicular check failed");
    
    CHECK(res);
    
    // Verify log file was created
    std::ifstream log_file("projgeom.log");
    CHECK(log_file.good());
    
    if (log_file.is_open()) {
        log_file.close();
        std::cout << "✓ Log file created successfully" << std::endl;
    }
    
    std::cout << "=== Spdlogger integration test completed ===" << std::endl;
}