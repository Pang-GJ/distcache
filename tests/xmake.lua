add_requires("doctest")
add_requires("fmt")

target("test_lru")
  set_kind("binary")
  set_group("tests")
  add_files("$(projectdir)/tests/test_lru.cpp")
  add_packages("doctest", "fmt")
  add_deps("distcache")


