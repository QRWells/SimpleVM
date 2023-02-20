add_defines("TEST")

add_requires("fmt", "magic_enum", "ctre")

target("Machine-test")
    set_default(false)
    set_kind("binary")
    set_group("test")

    add_includedirs("$(curdir)/include")

    add_files("$(curdir)/test/Machine/*.cc")
    add_files("$(curdir)/src/**.cc")
    remove_files("$(curdir)/src/main.cc")
    remove_files("$(curdir)/src/Assembler/Main.cc")

    add_packages("fmt", "magic_enum", "ctre")
target_end()

target("Assembler-test")
    set_default(false)
    set_kind("binary")
    set_group("test")

    add_includedirs("$(curdir)/include")

    add_files("$(curdir)/test/Assembler/*.cc")
    add_files("$(curdir)/src/**.cc")
    remove_files("$(curdir)/src/main.cc")
    remove_files("$(curdir)/src/Assembler/Main.cc")

    add_packages("fmt", "magic_enum", "ctre")
target_end()