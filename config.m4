dnl Tell PHP about the argument to enable the tru timeout extension

PHP_ARG_ENABLE(trutimeout, Whether to enable the tru timeout extension, [ --enable-tru-timeout   Enable tru timeout])

if test "$PHP_TRUTIMEOUT" != "no"; then
    PHP_NEW_EXTENSION(trutimeout, src/timeout.c, $ext_shared)
fi