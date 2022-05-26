#include <php.h>
#include <signal.h>
#include <sys/time.h>
#include <stdbool.h>
#include "zend_exceptions.h"
#include "zend_signal.h"
#include "timeout.h"


zend_function_entry trutimeout_functions[] = {
            PHP_FE(enableTimeout, NULL)
            PHP_FE(getTimeUntilTimeout, NULL)
        PHP_FE_END
};

zend_module_entry trutimeout_module_entry = {
            STANDARD_MODULE_HEADER,
        PHP_TRUTIMEOUT_EXTNAME,
        trutimeout_functions,
        PHP_MINIT(trutimeout),
        NULL,
        PHP_RINIT(trutimeout),
        PHP_RSHUTDOWN(trutimeout),
        NULL,
        PHP_TRUTIMEOUT_VERSION,
        STANDARD_MODULE_PROPERTIES,
};

ZEND_GET_MODULE(trutimeout);

// Timeout exception creation
static zend_class_entry *timeout_exception_ce;

static zend_object *timeout_exception_create_object(zend_class_entry *ce) {
    zend_object *obj = zend_ce_exception->create_object(ce);
    zval obj_zv, rv, *trace;

    return obj;
}



zend_long seconds = 0;
struct itimerval timeout_timer;
bool timeout_enabled;

int my_pid;

void disable_timeout() {
    if (timeout_enabled == true) {
        timeout_enabled = false;
        // Settime both values of it_value to 0 to disable the timer
        timeout_timer.it_value.tv_sec = timeout_timer.it_value.tv_usec = timeout_timer.it_interval.tv_sec = timeout_timer.it_interval.tv_usec = 0;
        setitimer(ITIMER_REAL, &timeout_timer, NULL);
    }
}

void handle_timeout(int sig)
{
    disable_timeout();

    char message[64];
    sprintf(message, "Timeout of %ld seconds exceeded", seconds);
    zend_throw_exception(timeout_exception_ce, message, 124);
}

void enable_timeout() {
    my_pid = getpid();

    int signo;

    timeout_timer.it_value.tv_sec = seconds;
    timeout_timer.it_value.tv_usec = timeout_timer.it_interval.tv_sec = timeout_timer.it_interval.tv_usec = 0;

    if (setitimer(ITIMER_REAL, &timeout_timer, NULL) == -1) {
        perror("Issue creating max execution timer");
        exit(1);
    }

    signal(SIGALRM, handle_timeout); 
    timeout_enabled = true;
}

PHP_FUNCTION(enableTimeout) {
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "l", &seconds) == FAILURE) {
        RETURN_NULL();
    }

    enable_timeout();
};

PHP_FUNCTION(getTimeUntilTimeout) {
    if (zend_parse_parameters(ZEND_NUM_ARGS() TSRMLS_CC, "") == FAILURE) {
        RETURN_NULL();
    }

    if (timeout_enabled == true) {
        struct itimerval cur_timer;
        getitimer(ITIMER_REAL, &cur_timer);
        RETURN_DOUBLE(cur_timer.it_value.tv_usec / 1000000.0 + cur_timer.it_value.tv_sec);
    } else {
        RETURN_DOUBLE(0);
    }
};


PHP_MINIT_FUNCTION(trutimeout) {
    timeout_exception_ce = register_class_TimeoutException(zend_ce_exception);
    timeout_exception_ce->create_object = timeout_exception_create_object;

    // Register signal handler
    signal(SIGALRM, handle_timeout);
    if (getpid() != my_pid && seconds > 0) {
        printf("trutimeout_minit\n");
        enable_timeout();
    }
    return SUCCESS;
}

PHP_RSHUTDOWN_FUNCTION(trutimeout) {
    disable_timeout();
    return SUCCESS;
}

PHP_RINIT_FUNCTION(trutimeout) {
    if (getpid() != my_pid && seconds > 0) {
        printf("trutimeout_rinit\n");
        enable_timeout();
        return SUCCESS;
    }
    disable_timeout();
    return SUCCESS;
}