AC_INIT([RedisILayer], [0.9.0], [fnchooft@gmail.com], [RedisILayer],
        [http://github.com/fnchooft/RedisILayer])
AC_PREREQ([2.59])

AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])

PACKAGE_REQUIRES=`./req.sh`
AC_SUBST([PACKAGE_REQUIRES],[$PACKAGE_REQUIRES])
CXXFLAGS="$CFLAGS "
LIBS="$LIBS "

PKG_CHECK_MODULES([PKGS], [${PACKAGE_REQUIRES}])
CXXFLAGS+=" $PKGS_CFLAGS"
LIBS+=" $PKGS_LIBS"

# From https://nerdland.net/2009/07/detecting-c-libraries-with-autotools
AC_CHECK_LIB([gtest_main], [main],
  [HAVE_GTEST=1] [GTEST_LIBS="$TEST_LIBS -lgtest -lpthread -lgtest_main"],
  AC_MSG_WARN([libgtest is not installed.]))

if test -z "$HAVE_GTEST";
  then PKG_CHECK_MODULES([GTEST], [gtest-1.6.0], [HAVE_GTEST_PKG=1],  [HAVE_GTEST_PKG=0])
fi
AM_CONDITIONAL([RUN_UNITTEST], [ test "x$HAVE_GTEST_PKG" = "xtrue" ])

AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
LT_INIT([static])

AC_CONFIG_FILES([Makefile ${PACKAGE_NAME}-${PACKAGE_VERSION}.pc:pkg.pc.in])
AC_CONFIG_FILES([src/Makefile])
AC_OUTPUT
