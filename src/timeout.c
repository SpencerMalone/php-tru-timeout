#include <php.h>
#include <signal.h>
#include "timeout.h"


zend_function_entry trutimeout_functions[] = {
            PHP_FE(enable_timeout, NULL)
        PHP_FE_END
};

zend_module_entry trutimeout_module_entry = {
            STANDARD_MODULE_HEADER,
        PHP_TRUTIMEOUT_EXTNAME,
        trutimeout_functions,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        PHP_TRUTIMEOUT_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(trutimeout);

void handle_exit(int sig)
{
    php_printf("Process timeout!\n");
    exit(124);
}
  

PHP_FUNCTION(enable_timeout) {
    zend_long timeout;

    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &timeout) == FAILURE) {
        RETURN_NULL();
    }

    pid_t childPID = fork();
    if (childPID==0) {
        sleep(timeout);
        kill(getppid(), SIGTERM);
        kill(getppid(), SIGKILL);
        _exit(0);
    } else {
        signal(SIGTERM, handle_exit); // This is needed to handle us being PID 1 in a containerized environment 
    }
};