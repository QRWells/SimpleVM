set_xmakever("2.6.3")

add_defines("ROOT")
set_project("SimpleVM")
set_version("0.0.1", {build = "%Y%m%d%H%M"})
set_toolchains("clang")
set_languages("c17", "gnu++20")

if is_mode("debug") then
    add_defines("DEBUG")
end

if is_mode("debug") then
    add_defines("DEBUG")
    set_symbols("debug")
    set_optimize("none")
elseif is_mode("release") then
    set_symbols("hidden")
    set_strip("all")
    set_optimize("fastest")
    add_cxflags("-fomit-frame-pointer")
    add_mxflags("-fomit-frame-pointer")
end

includes("src", "test")