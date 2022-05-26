#define PHP_TRUTIMEOUT_EXTNAME "trutimeout"
#define PHP_TRUTIMEOUT_VERSION "0.0.1"

PHP_MINIT_FUNCTION(trutimeout);
PHP_RINIT_FUNCTION(trutimeout);
PHP_RSHUTDOWN_FUNCTION(trutimeout);
PHP_FUNCTION(enableTimeout);
PHP_FUNCTION(getTimeUntilTimeout);

static const zend_function_entry class_TimeoutException_methods[] = {
	ZEND_FE_END
};

static zend_class_entry *register_class_TimeoutException(zend_class_entry *class_entry_Exception)
{
	zend_class_entry ce, *class_entry;

	INIT_CLASS_ENTRY(ce, "TimeoutException", class_TimeoutException_methods);
	class_entry = zend_register_internal_class_ex(&ce, class_entry_Exception);

	return class_entry;
}