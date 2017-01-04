AC_INIT([RedisILayer], [0.9.0], [fnchooft@gmail.com], [RedisILayer],
        [http://github.com/fnchooft/RedisILayer])
AC_PREREQ([2.59])

AM_INIT_AUTOMAKE([1.10 -Wall no-define foreign])

PACKAGE_REQUIRES=`./req.sh`

CXXFLAGS="$CFLAGS "
LIBS="$LIBS "

PKG_CHECK_MODULES([PKGS], [${PACKAGE_REQUIRES}])
CXXFLAGS+=" $PKGS_CFLAGS"
LIBS+=" $PKGS_LIBS"    

AC_CONFIG_HEADERS([config.h])
AC_PROG_CXX
LT_INIT([static])

AC_CONFIG_FILES([Makefile ${PACKAGE_NAME}-${PACKAGE_VERSION}.pc:pkg.pc.in])
AC_OUTPUT
