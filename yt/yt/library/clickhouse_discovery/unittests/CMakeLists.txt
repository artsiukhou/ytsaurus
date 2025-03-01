
# This file was generated by the build system used internally in the Yandex monorepo.
# Only simple modifications are allowed (adding source-files to targets, adding simple properties
# like target_include_directories). These modifications will be ported to original
# ya.make files by maintainers. Any complex modifications which can't be ported back to the
# original buildsystem will not be accepted.



add_executable(unittester-library-clickhouse-discovery)
target_compile_options(unittester-library-clickhouse-discovery PRIVATE
  -Wdeprecated-this-capture
)
target_link_libraries(unittester-library-clickhouse-discovery PUBLIC
  contrib-libs-linux-headers
  contrib-libs-cxxsupp
  yutil
  library-cpp-cpuid_check
  cpp-testing-gtest
  cpp-testing-gtest_main
  yt-yt-build
  client-unittests-mock
  yt-yt-core
  yt-core-test_framework
  yt-library-clickhouse_discovery
)
target_link_options(unittester-library-clickhouse-discovery PRIVATE
  -ldl
  -lrt
  -Wl,--no-as-needed
  -fPIC
  -fPIC
  -lpthread
  -lrt
  -ldl
  -lutil
)
target_sources(unittester-library-clickhouse-discovery PRIVATE
  ${CMAKE_SOURCE_DIR}/yt/yt/library/clickhouse_discovery/unittests/discovery_ut.cpp
  ${CMAKE_SOURCE_DIR}/yt/yt/library/clickhouse_discovery/unittests/helpers_ut.cpp
)
add_test(
  NAME
  unittester-library-clickhouse-discovery
  COMMAND
  unittester-library-clickhouse-discovery
)
set_property(
  TEST
  unittester-library-clickhouse-discovery
  PROPERTY
  LABELS
  MEDIUM
)
set_property(
  TEST
  unittester-library-clickhouse-discovery
  PROPERTY
  PROCESSORS
  1
)
set_property(
  TEST
  unittester-library-clickhouse-discovery
  PROPERTY
  ENVIRONMENT
)
target_allocator(unittester-library-clickhouse-discovery
  cpp-ytalloc-impl
)
vcs_info(unittester-library-clickhouse-discovery)
