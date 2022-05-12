## php-tru-timeout

Run `./script/test` with docker installed and running.

The goal here is simple: Enforce a PHP timeout that actually respects `sleep` and other external commands properly on linux.

PHP max execution time normally has the following caveat:
> On non Windows systems, the maximum execution time is not affected by system calls, stream operations etc.

It's 2022, this exemption for non-windows systems is ridiculous.


### How does it work?

First, it forks. After the fork, the parent process continues living a normal PHP life. Meanwhile, the child watches the execution time of the parent, and if it exceeds a set value, it kills the parent.