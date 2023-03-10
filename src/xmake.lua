add_requires("spdlog", {system=false, configs = {header_only = true, fmt_external=true}})
add_requireconfs("spdlog.fmt", {override = true, version = "9.1.0", configs = {header_only = true}})
add_requires("nlohmann_json", {system=false})

target("distcache")
  set_kind("shared")
  add_files("$(projectdir)/src/**.cpp")
  add_includedirs("$(projectdir)/src", { public = true})
  add_packages("fmt", "spdlog")
target_end()
