## php-tru-timeout

Run `./script/test` with docker installed and running.

The goal here is simple: Enforce a PHP timeout that actually respects `sleep` and other external commands properly on linux.

PHP max execution time normally has the following caveat:
> On non Windows systems, the maximum execution time is not affected by system calls, stream operations etc.

It's 2022, this exemption for non-windows systems is ridiculous.


### How does it work?

The implementation is based off of PHP's source @ https://github.com/php/php-src/blob/da857c94a40bc1311d8598ddedc30b1d49419790/Zend/zend_execute_API.c#L1452-L1466. We use `setitimer` with the parameter `ITIMER_REAL` instead of `ITIMER_PROF`, and then handle the new signal `SIGALRM`.