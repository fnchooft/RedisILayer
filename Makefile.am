
lib_LIBRARIES = lib@PACKAGE_NAME@-@PACKAGE_VERSION@.a
@PACKAGE_NAME@_@PACKAGE_VERSION@_includedir = $(includedir)/@PACKAGE_NAME@-@PACKAGE_VERSION@

# Add local RedisHelper - probably should become a seperate library.
# that way we can encapsulate hiredis and libev
lib@PACKAGE_NAME@_@PACKAGE_VERSION@_a_SOURCES = src/RedisHelper.cpp

# check_PROGRAMS = structure_test
# TESTS = structure_test
# GTESTLIB=$(shell pkg-config gtest-1.6.0 --libs)
# GTESTFLAGS=$(shell pkg-config gtest-1.6.0 --cflags)
# structure_test_CXXFLAGS= $(GTESTFLAGS)
# structure_test_LDFLAGS= $(GTESTLIB) -L. -l@PACKAGE_NAME@-@PACKAGE_VERSION@   -lgtest_main
# structure_test_SOURCES=StructureTests.cpp
