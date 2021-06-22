/* morse extension for PHP */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_morse.h"
#include "morse_arginfo.h"

/* For compatibility with older PHP versions */
#ifndef ZEND_PARSE_PARAMETERS_NONE
#define ZEND_PARSE_PARAMETERS_NONE() \
	ZEND_PARSE_PARAMETERS_START(0, 0) \
	ZEND_PARSE_PARAMETERS_END()
#endif

ZEND_DECLARE_MODULE_GLOBALS(morse)

/* {{{ void test1() */
PHP_FUNCTION(test1)
{
	ZEND_PARSE_PARAMETERS_NONE();

	php_printf("The extension %s is loaded and working!\r\n", "morse");
}
/* }}} */

/* {{{ string test2( [ string $var ] ) */
PHP_FUNCTION(test2)
{
	char *var = "World";
	size_t var_len = sizeof("World") - 1;
	zend_string *retval;

	ZEND_PARSE_PARAMETERS_START(0, 1)
		Z_PARAM_OPTIONAL
		Z_PARAM_STRING(var, var_len)
	ZEND_PARSE_PARAMETERS_END();

	retval = strpprintf(0, "Hello %s", var);

	RETURN_STR(retval);
}
/* }}}*/

static int do_scale(zval *return_value, zval *x, zend_long factor) {
    ZVAL_DEREF(x);
    if (Z_TYPE_P(x) == IS_LONG) {
        RETVAL_LONG(Z_LVAL_P(x) * factor);
    } else if (Z_TYPE_P(x) == IS_DOUBLE) {
        RETVAL_DOUBLE(Z_DVAL_P(x) * factor);
    } else if (Z_TYPE_P(x) == IS_STRING) {
        zend_string *ret = zend_string_safe_alloc(Z_STRLEN_P(x), factor, 0, 0);
        char *p = ZSTR_VAL(ret);
        
        while (factor-- > 0) {
            memcpy(p, Z_STRVAL_P(x), Z_STRLEN_P(x));
            p += Z_STRLEN_P(x);
        }
        *p = '\0';
        RETVAL_STR(ret);
    } else if (Z_TYPE_P(x) == IS_ARRAY) {
        zend_array *ret = zend_new_array(zend_array_count(Z_ARR_P(x)));
        zend_ulong idx;
        zend_string *key;
        zval *val, tmp;

        ZEND_HASH_FOREACH_KEY_VAL(Z_ARR_P(x), idx, key, val) {
            if (do_scale(&tmp, val, factor) != SUCCESS) {
                return FAILURE;
            }
            if (key) {
                zend_hash_add(ret, key, &tmp);
            } else {
                zend_hash_index_add(ret, idx, &tmp);
            }
        } ZEND_HASH_FOREACH_END();
        RETVAL_ARR(ret);
    } else {
        php_error_docref(NULL, E_WARNING, "unexpected argument type");
        return FAILURE;
    }
    return SUCCESS;
}

static int do_scale_ref(zval *x, zend_long factor) {
    ZVAL_DEREF(x);
    if (Z_TYPE_P(x) == IS_LONG) {
        Z_LVAL_P(x) *= factor;
    } else if (Z_TYPE_P(x) == IS_DOUBLE) {
        Z_DVAL_P(x) *= factor;
    } else if (Z_TYPE_P(x) == IS_STRING) {
        size_t len = Z_STRLEN_P(x);
        char *p;
        
        ZVAL_STR(x, zend_string_safe_realloc(Z_STR_P(x), len, factor, 0, 0));
        p = Z_STRVAL_P(x) + len;
        while (--factor > 0) {
            memcpy(p, Z_STRVAL_P(x), len);
            p += len;
        }
        *p = '\0';
    } else if (Z_TYPE_P(x) == IS_ARRAY) {
        zval *val;
        
        SEPARATE_ARRAY(x);
        ZEND_HASH_FOREACH_VAL(Z_ARR_P(x), val) {
            if (do_scale_ref(val, factor) != SUCCESS) {
                return FAILURE;
            }
        } ZEND_HASH_FOREACH_END();
    } else {
        php_error_docref(NULL, E_WARNING, "unexpected argument type");
        return FAILURE;
    }
    return SUCCESS;
}

PHP_FUNCTION(test_scale) {
    zval *x;
    zend_long factor = TEST_G(scale);
    
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(x)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(factor)
    ZEND_PARSE_PARAMETERS_END();

    /*if (Z_TYPE_P(x) == IS_LONG) {
        RETURN_LONG(Z_LVAL_P(x) * factor);
    } else if (Z_TYPE_P(x) == IS_DOUBLE) {
        RETURN_DOUBLE(Z_DVAL_P(x) * factor);
    } else if (Z_TYPE_P(x) == IS_STRING) {
        zend_string *ret = zend_string_safe_alloc(Z_STRLEN_P(x), factor, 0, 0);
        char *p = ZSTR_VAL(ret);
        while (factor-- > 0) {
            memcpy(p, Z_STRVAL_P(x), Z_STRLEN_P(x));
            p += Z_STRLEN_P(x);
        }
        *p = '\000';
        RETURN_STR(ret);
    } else {
        php_error_docref(NULL, E_WARNING, "unexcepted argument type");
        return;
    }*/
    do_scale(return_value, x, factor);
}

PHP_FUNCTION(test_scale_ref) {
    zval *x;
    zend_long factor = TEST_G(scale);
    
    ZEND_PARSE_PARAMETERS_START(1, 2)
        Z_PARAM_ZVAL(x)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(factor)
    ZEND_PARSE_PARAMETERS_END();

    do_scale_ref(x, factor);
}


static zend_class_entry *scaler_class_entry = NULL;

#define DEFAULT_SCALE_FACTOR 2

PHP_METHOD(Scaler, __construct) {
    zend_long factor = DEFAULT_SCALE_FACTOR;

    ZEND_PARSE_PARAMETERS_START(0, 1)
        Z_PARAM_OPTIONAL
        Z_PARAM_LONG(factor)
    ZEND_PARSE_PARAMETERS_END();
    
    if (ZEND_NUM_ARGS() > 0) {
        zend_update_property_long(Z_OBJCE_P(ZEND_THIS), Z_OBJ_P(ZEND_THIS), "factor", sizeof("factor")-1, factor);
    }
}

PHP_METHOD(Scaler, scale) {
    zval *x, *zv, tmp;
    zend_long factor;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ZVAL(x)
    ZEND_PARSE_PARAMETERS_END();

    zv = zend_read_property(Z_OBJCE_P(ZEND_THIS), Z_OBJ_P(ZEND_THIS), "factor", sizeof("factor")-1, 0, &tmp);
    factor = zval_get_long(zv);
    
    do_scale_ref(x, factor);
}


static PHP_GINIT_FUNCTION(morse) {
#if defined(COMPILE_DL_BCMATH) && defined(ZTS)
    ZEND_TSRMLS_CACHE_UPDATE();
#endif
    morse_globals->scale = 1;
}

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("morse.scale", "1", PHP_INI_ALL, OnUpdateLong, scale, zend_morse_globals, morse_globals)
PHP_INI_END()

PHP_MINIT_FUNCTION(morse) {
    zend_class_entry ce;

    REGISTER_INI_ENTRIES();
    REGISTER_LONG_CONSTANT("TEST_SCALE_FACTOR", 2, CONST_CS | CONST_PERSISTENT);

    INIT_CLASS_ENTRY(ce, "Scaler", scaler_functions);
    scaler_class_entry = zend_register_internal_class(&ce);
    
    zend_declare_class_constant_long(scaler_class_entry, "DEFAULT_FACTOR", sizeof("DEFAULT_FACTOR")-1, DEFAULT_SCALE_FACTOR);
    
    zend_declare_property_long(scaler_class_entry, "factor", sizeof("factor")-1, DEFAULT_SCALE_FACTOR, ZEND_ACC_PRIVATE);
    return SUCCESS;
}

/* {{{ PHP_RINIT_FUNCTION */
PHP_RINIT_FUNCTION(morse)
{
#if defined(ZTS) && defined(COMPILE_DL_MORSE)
	ZEND_TSRMLS_CACHE_UPDATE();
#endif

	return SUCCESS;
}
/* }}} */

/* {{{ PHP_MINFO_FUNCTION */
PHP_MINFO_FUNCTION(morse)
{
	php_info_print_table_start();
	php_info_print_table_header(2, "morse support", "enabled");
	php_info_print_table_end();
}
/* }}} */

/* {{{ morse_module_entry */
zend_module_entry morse_module_entry = {
	STANDARD_MODULE_HEADER,
	"morse",					/* Extension name */
	ext_functions,					/* zend_function_entry */
	PHP_MINIT(morse),							/* PHP_MINIT - Module initialization */
	NULL,							/* PHP_MSHUTDOWN - Module shutdown */
	PHP_RINIT(morse),			/* PHP_RINIT - Request initialization */
	NULL,							/* PHP_RSHUTDOWN - Request shutdown */
	PHP_MINFO(morse),			/* PHP_MINFO - Module info */
	PHP_MORSE_VERSION,		/* Version */
    PHP_MODULE_GLOBALS(morse),  //mdule globals
    PHP_GINIT(morse),
    NULL,
    NULL,
	STANDARD_MODULE_PROPERTIES_EX
};
/* }}} */

#ifdef COMPILE_DL_MORSE
# ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
# endif
ZEND_GET_MODULE(morse)
#endif
