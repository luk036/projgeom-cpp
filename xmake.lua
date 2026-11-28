add_rules("mode.debug", "mode.release", "mode.coverage")
add_requires("fmt", "doctest", "nanobench", "spdlog")

set_languages("c++20")
add_includedirs("include", "../fractions-cpp/include")

if is_mode("coverage") then
    add_cxflags("-ftest-coverage", "-fprofile-arcs", {force = true})
end

if is_plat("linux") then
    add_syslinks("pthread")
    -- Termux/Android ships its libc++ headers under $PREFIX
    local prefix = os.getenv("PREFIX")
    if prefix then
        add_cxflags("-Wno-unused-command-line-argument", {force = true})
        add_sysincludedirs(prefix .. "/include/c++/v1", prefix .. "/include", {public = true})
    end
elseif is_plat("windows") then
    add_cxflags("/utf-8 /W4 /WX /wd4702", {force = true})
end

target("test_projgeom")
    add_files("test/source/*.cpp", "source/*.cpp")
    add_packages("fmt", "doctest", "spdlog")
    add_tests("default")

    -- rapidcheck is optionally provided by a CMake build
    local rapidcheck_dir = path.join(os.projectdir(), "build", "_deps", "rapidcheck-src")
    local rapidcheck_lib_dir = path.join(os.projectdir(), "build", "_deps", "rapidcheck-build")
    if is_plat("windows") then
        rapidcheck_lib_dir = path.join(rapidcheck_lib_dir, "Release")
    end

    if os.isdir(rapidcheck_dir) and os.isdir(rapidcheck_lib_dir) then
        add_includedirs(path.join(rapidcheck_dir, "include"))
        add_linkdirs(rapidcheck_lib_dir)
        add_links("rapidcheck")
        add_defines("RAPIDCHECK_H")
    end

target("BM_projgeom")
    add_files("bench/BM_projgeom.cpp")
    add_packages("nanobench", "fmt", "spdlog")

target("fold_trick")
    add_files("examples/fold_trick.cpp")
    if is_plat("windows") then
        add_cxflags("/utf-8 /W4 /wd4702", {force = true})
    else
        add_cxflags("-Wall -Wextra", {force = true})
    end
