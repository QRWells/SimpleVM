set_xmakever("2.6.3")

add_defines("ROOT")
set_project("SimpleVM")
set_version("0.0.1", {build = "%Y%m%d%H%M"})
set_toolchains("clang")

includes("src")
