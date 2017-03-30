lib_LTLIBRARIES = lib@PACKAGE_NAME@-@PACKAGE_VERSION@.la
@PACKAGE_NAME@_@PACKAGE_VERSION@_includedir = $(includedir)/@PACKAGE_NAME@-@PACKAGE_VERSION@

lib@PACKAGE_NAME@_@PACKAGE_VERSION@_la_SOURCES = RedisHelper.cpp
@PACKAGE_NAME@_@PACKAGE_VERSION@_include_HEADERS = RedisHelper.hpp

if RUN_UNITTEST
check_PROGRAMS = RedisILayer_test
TESTS = RedisILayer_test

RedisILayer_test_CXXFLAGS= $(GTEST_CFLAGS)
RedisILayer_test_LDFLAGS= $(GTEST_LIBS) -L. -l@PACKAGE_NAME@-@PACKAGE_VERSION@ -lgtest_main
RedisILayer_test_SOURCES= RedisILayer_test.cpp
endif