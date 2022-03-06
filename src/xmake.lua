add_defines("SRC")

target("SimpleVM")
    set_default(true)
    set_kind("binary")
    set_languages("c17", "gnu++20")

    add_includedirs("$(curdir)/include")

    add_files("**.cc")
target_end()