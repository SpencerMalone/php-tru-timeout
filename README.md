## php-tru-timeout

Run `./script/test` with docker installed and running.

The goal here is simple: Enforce a PHP timeout that actually respects `sleep` and other external commands properly on linux.

PHP max execution time normally has the following caveat:
> On non Windows systems, the maximum execution time is not affected by system calls, stream operations etc.

It's 2022, this exemption for non-windows systems is ridiculous.


### How does it work?

The implementation is based off of PHP's source @ https://github.com/php/php-src/blob/da857c94a40bc1311d8598ddedc30b1d49419790/Zend/zend_execute_API.c#L1452-L1466. We use `setitimer` with the parameter `ITIMER_REAL` instead of `ITIMER_PROF`, and then handle the new signal `SIGALRM`.


### Is it safe?

According to https://github.com/php/php-src/pull/6504#issuecomment-870986910, Rasmus Lerdorf (co-author of PHP) says the only reason they did not use `ITIMER_REAL` was as follows:
> I wrote this initial code 25 years ago. At that time I couldn't use ITIMER_REAL because Apache used alarm() internally for timing out requests and I'd mess that up by catching the SIGALRM in PHP. SIGPROF had no such conflicts so that is why we have ITIMER_PROF today. I don't think modern Apache uses SIGALRM, so it should be possible now, but be careful of possible SAPIs that might live in an environment where you can't just steal SIGALRM.

Outside of that, production use has not been tested, so this is still use at your own risk.

### How do I use it?

`enableTimeout(<int>)` to enable a timeout. Pass in `0` to disable timeouts.


`getTimeUntilTimeout()` to get the time until timeout remaining as a double. Intended use case is if you short term wanted a lower timeout.

### TODO

- Make this a proper namespace'd class.
- Document