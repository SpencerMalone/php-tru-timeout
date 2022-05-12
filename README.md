## php-tru-timeout

Run `./script/test` with docker installed and running.

The goal here is simple: Enforce a PHP timeout that actually respects `sleep` and other external commands properly on linux.

PHP max execution time normally has the following caveat:
```
max_execution_time int
This sets the maximum time in seconds a script is allowed to run before it is terminated by the parser. This helps prevent poorly written scripts from tying up the server. The default setting is 30. When running PHP from the command line the default setting is 0.

On non Windows systems, the maximum execution time is not affected by system calls, stream operations etc.
```

It's 2022, this exemption for non-windows systems is ridiculous.