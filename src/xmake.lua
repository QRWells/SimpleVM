add_defines("SRC")

add_requires("fmt", "magic_enum", "ctre")

target("SimpleVM")
    set_default(true)
    set_kind("binary")

    add_includedirs("$(curdir)/include")

    add_files("**.cc")
	remove_files("$(curdir)/src/Assembler/*.cc")

    add_packages("fmt", "magic_enum", "ctre")
target_end()

target("SimpleVM-as")
    set_kind("binary")

    add_includedirs("$(curdir)/include")

    add_files("$(curdir)/src/Assembler/**.cc")

    add_packages("fmt", "magic_enum", "ctre")
target_end()