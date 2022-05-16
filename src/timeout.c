#include <php.h>
#include <signal.h>
#include <sys/time.h>
#include <stdbool.h>
#include "zend_exceptions.h"
#include "zend_signal.h"
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
        PHP_RINIT(trutimeout),
        PHP_RSHUTDOWN(trutimeout),
        NULL,
        PHP_TRUTIMEOUT_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(trutimeout);

zend_long seconds = 0;
struct itimerval timeout_timer;
bool timeout_enabled;

void disable_timeout() {
    if (timeout_enabled == true) {
        timeout_enabled = false;
        // Settime both values of it_value to 0 to disable the timer
        timeout_timer.it_value.tv_sec = timeout_timer.it_value.tv_usec = timeout_timer.it_interval.tv_sec = timeout_timer.it_interval.tv_usec = 0;
        setitimer(ITIMER_REAL, &timeout_timer, NULL);
    }
}

PHP_RSHUTDOWN_FUNCTION(trutimeout) {
    disable_timeout();
    return SUCCESS;
}

PHP_RINIT_FUNCTION(trutimeout) {
    disable_timeout();
    return SUCCESS;
}

void handle_timeout(int sig)
{
    disable_timeout();

    char message[64];
    sprintf(message, "Timeout of %ld seconds exceeded", seconds);
    zend_throw_exception(NULL, message, 124);
}

PHP_FUNCTION(enable_timeout) {
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
        RETURN_NULL();
    }

    int signo;

    timeout_timer.it_value.tv_sec = seconds;
    timeout_timer.it_value.tv_usec = timeout_timer.it_interval.tv_sec = timeout_timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timeout_timer, NULL) == -1) {
        perror("Issue creating max execution timer");
        exit(1);
    }

    signal(SIGALRM, handle_timeout); 
    timeout_enabled = true;
};
