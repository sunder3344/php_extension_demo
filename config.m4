dnl config.m4 for extension morse

dnl Comments in this file start with the string 'dnl'.
dnl Remove where necessary.

dnl If your extension references something external, use 'with':

dnl PHP_ARG_WITH([morse],
dnl   [for morse support],
dnl   [AS_HELP_STRING([--with-morse],
dnl     [Include morse support])])

dnl Otherwise use 'enable':

PHP_ARG_ENABLE([morse],
  [whether to enable morse support],
  [AS_HELP_STRING([--enable-morse],
    [Enable morse support])],
  [no])

if test "$PHP_MORSE" != "no"; then
  dnl Write more examples of tests here...

  dnl Remove this code block if the library does not support pkg-config.
  dnl PKG_CHECK_MODULES([LIBFOO], [foo])
  dnl PHP_EVAL_INCLINE($LIBFOO_CFLAGS)
  dnl PHP_EVAL_LIBLINE($LIBFOO_LIBS, MORSE_SHARED_LIBADD)

  dnl If you need to check for a particular library version using PKG_CHECK_MODULES,
  dnl you can use comparison operators. For example:
  dnl PKG_CHECK_MODULES([LIBFOO], [foo >= 1.2.3])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo < 3.4])
  dnl PKG_CHECK_MODULES([LIBFOO], [foo = 1.2.3])

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-morse -> check with-path
  dnl SEARCH_PATH="/usr/local /usr"     # you might want to change this
  dnl SEARCH_FOR="/include/morse.h"  # you most likely want to change this
  dnl if test -r $PHP_MORSE/$SEARCH_FOR; then # path given as parameter
  dnl   MORSE_DIR=$PHP_MORSE
  dnl else # search default path list
  dnl   AC_MSG_CHECKING([for morse files in default path])
  dnl   for i in $SEARCH_PATH ; do
  dnl     if test -r $i/$SEARCH_FOR; then
  dnl       MORSE_DIR=$i
  dnl       AC_MSG_RESULT(found in $i)
  dnl     fi
  dnl   done
  dnl fi
  dnl
  dnl if test -z "$MORSE_DIR"; then
  dnl   AC_MSG_RESULT([not found])
  dnl   AC_MSG_ERROR([Please reinstall the morse distribution])
  dnl fi

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-morse -> add include path
  dnl PHP_ADD_INCLUDE($MORSE_DIR/include)

  dnl Remove this code block if the library supports pkg-config.
  dnl --with-morse -> check for lib and symbol presence
  dnl LIBNAME=MORSE # you may want to change this
  dnl LIBSYMBOL=MORSE # you most likely want to change this

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   AC_DEFINE(HAVE_MORSE_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your morse library.])
  dnl ], [
  dnl   $LIBFOO_LIBS
  dnl ])

  dnl If you need to check for a particular library function (e.g. a conditional
  dnl or version-dependent feature) and you are not using pkg-config:
  dnl PHP_CHECK_LIBRARY($LIBNAME, $LIBSYMBOL,
  dnl [
  dnl   PHP_ADD_LIBRARY_WITH_PATH($LIBNAME, $MORSE_DIR/$PHP_LIBDIR, MORSE_SHARED_LIBADD)
  dnl   AC_DEFINE(HAVE_MORSE_FEATURE, 1, [ ])
  dnl ],[
  dnl   AC_MSG_ERROR([FEATURE not supported by your morse library.])
  dnl ],[
  dnl   -L$MORSE_DIR/$PHP_LIBDIR -lm
  dnl ])
  dnl
  dnl PHP_SUBST(MORSE_SHARED_LIBADD)

  dnl In case of no dependencies
  AC_DEFINE(HAVE_MORSE, 1, [ Have morse support ])

  PHP_NEW_EXTENSION(morse, morse.c, $ext_shared)
fi
