
lib_LIBRARIES = lib@PACKAGE_NAME@-@PACKAGE_VERSION@.a
@PACKAGE_NAME@_@PACKAGE_VERSION@_includedir = $(includedir)/@PACKAGE_NAME@-@PACKAGE_VERSION@

include padtec_lightpad_infra_manager.mk

lib@PACKAGE_NAME@_@PACKAGE_VERSION@_a_SOURCES = $(padtec_lightpad_infra_manager_lib_sources)
@PACKAGE_NAME@_@PACKAGE_VERSION@_include_HEADERS = $(padtec_lightpad_infra_manager_lib_headers)

# Add local RedisHelper - probably should become a seperate library.
# that way we can encapsulate hiredis and libev
lib@PACKAGE_NAME@_@PACKAGE_VERSION@_a_SOURCES += padtec_lightpad_infra_manager/handlers/RedisHelper.cpp

bin_PROGRAMS = padtec_lightpad_infra_manager_client padtec_lightpad_infra_manager_server

padtec_lightpad_infra_manager_client_SOURCES = $(padtec_lightpad_infra_manager_cli_sources)
padtec_lightpad_infra_manager_client_LDFLAGS = -L. -l@PACKAGE_NAME@-@PACKAGE_VERSION@

padtec_lightpad_infra_manager_server_SOURCES = $(padtec_lightpad_infra_manager_app_sources)
padtec_lightpad_infra_manager_server_LDFLAGS = -L. -l@PACKAGE_NAME@-@PACKAGE_VERSION@


check_PROGRAMS = structure_test
TESTS = structure_test
GTESTLIB=$(shell pkg-config gtest-1.6.0 --libs)
GTESTFLAGS=$(shell pkg-config gtest-1.6.0 --cflags)
structure_test_CXXFLAGS= $(GTESTFLAGS)
structure_test_LDFLAGS= $(GTESTLIB) -L. -l@PACKAGE_NAME@-@PACKAGE_VERSION@   -lgtest_main
structure_test_SOURCES=padtec_lightpad_infra_manager/structures/PadtecLightpadInfraManagerStructureTests.cpp
