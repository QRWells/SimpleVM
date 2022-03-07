add_defines("SRC")

add_requires("fmt", "vcpkg::magic-enum")

if is_mode("debug") then
    add_defines("DEBUG")
end

target("SimpleVM")
    set_default(true)
    set_kind("binary")
    set_languages("c17", "gnu++20")

    add_includedirs("$(curdir)/include")

    add_files("**.cc")

    add_packages("fmt", "vcpkg::magic-enum")
target_end()