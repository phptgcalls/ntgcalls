dnl --enable-ntgcalls[--enable ntgcalls extension]
PHP_ARG_ENABLE(ntgcalls, whether to enable ntgcalls support,
[  --enable-ntgcalls           Enable ntgcalls extension])

if test "$PHP_NTG_CALLS" != "no"; then
  AC_MSG_CHECKING([for C++ compiler])
  AC_PROG_CXX
  if test -z "$CXX"; then
    AC_MSG_ERROR([No C++ compiler found])
  fi
  AC_MSG_RESULT([$CXX])

  
  dnl --- Begin building C libraries via CMake ---
  AC_MSG_NOTICE([Checking for cmake...])
  AC_PATH_PROG([CMAKE], [cmake])
  if test -z "$CMAKE"; then
    AC_MSG_ERROR([cmake is required to build the C libraries])
  fi

  dnl Define a build directory relative to the extension source
  BUILD_DIR="$srcdir/build"
  if test ! -d "$BUILD_DIR"; then
    AC_MSG_NOTICE([Creating build directory at $BUILD_DIR...])
    mkdir -p "$BUILD_DIR"
  fi

  dnl Run CMake configuration step (assumes CMakeLists.txt is in $srcdir)
  AC_MSG_NOTICE([Running cmake configuration...])
  $CMAKE -S "$srcdir" -B "$BUILD_DIR" || AC_MSG_ERROR([cmake configuration failed])

  dnl Build the C libraries using make
  AC_MSG_NOTICE([Building C libraries...])
  make -C "$BUILD_DIR" || AC_MSG_ERROR([make failed])

  dnl Install the built C libraries
  AC_MSG_NOTICE([Installing C libraries...])
  make -C "$BUILD_DIR" install || AC_MSG_ERROR([make install failed])
  dnl --- End building C libraries via CMake ---

  PHP_ADD_INCLUDE([$PHP_SRC_DIR])
  PHP_ADD_INCLUDE([$PHP_INSTALL/include/php])
  PHP_ADD_INCLUDE([$srcdir])
  PHP_NEW_EXTENSION(ntgcalls, ntgcalls/src/bindings/phpapi.cpp, $ext_shared, -lstdc++)

  AC_MSG_NOTICE([PHP Zend support (php.h, zend_API.h, etc.) is enabled])
fi
