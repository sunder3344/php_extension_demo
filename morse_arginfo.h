/* This is a generated file, edit the .stub.php file instead.
 * Stub hash: 7eb3fd4083c98e6dffc8b02b6373b7ce9cbf228d */

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test1, 0, 0, IS_VOID, 0)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_WITH_RETURN_TYPE_INFO_EX(arginfo_test2, 0, 0, IS_STRING, 0)
	ZEND_ARG_TYPE_INFO_WITH_DEFAULT_VALUE(0, str, IS_STRING, 0, "\"\"")
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_test_scale, 0)
    ZEND_ARG_INFO(0, x)
    ZEND_ARG_INFO(0, factor)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_test_scale_ref, 1)
    ZEND_ARG_INFO(1, x)
    ZEND_ARG_INFO(0, factor)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_scaler_construct, 0)
    ZEND_ARG_INFO(0, factor)
ZEND_END_ARG_INFO()

ZEND_BEGIN_ARG_INFO(arginfo_scaler_scale, 0)
    ZEND_ARG_INFO(1, x)
ZEND_END_ARG_INFO()

ZEND_FUNCTION(test1);
ZEND_FUNCTION(test2);
ZEND_FUNCTION(test_scale);
ZEND_FUNCTION(test_scale_ref);

ZEND_METHOD(Scaler, __construct);
ZEND_METHOD(Scaler, scale);


static const zend_function_entry ext_functions[] = {
	ZEND_FE(test1, arginfo_test1)
	ZEND_FE(test2, arginfo_test2)
    ZEND_FE(test_scale, arginfo_test_scale)
    ZEND_FE(test_scale_ref, arginfo_test_scale_ref)
	ZEND_FE_END
};

static const zend_function_entry scaler_functions[] = {
    ZEND_ME(Scaler, __construct, arginfo_scaler_construct, ZEND_ACC_PUBLIC)
    ZEND_ME(Scaler, scale, arginfo_scaler_scale, ZEND_ACC_PUBLIC)
    ZEND_FE_END
};
