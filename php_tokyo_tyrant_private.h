/*
   +----------------------------------------------------------------------+
   | PHP Version 5 / Tokyo tyrant                                         |
   +----------------------------------------------------------------------+
   | Copyright (c) 2009-2010 Mikko Koppanen                               |
   +----------------------------------------------------------------------+
   | This source file is dual-licensed.                                   |
   | It is available under the terms of New BSD License that is bundled   |
   | with this package in the file LICENSE and available under the terms  |
   | of PHP license version 3.01. PHP license is bundled with this        |
   | package in the file LICENSE and can be obtained through the          |
   | world-wide-web at the following url:                                 |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
   | Author: Mikko Kopppanen <mkoppanen@php.net>                          |
   +----------------------------------------------------------------------+
*/
#ifndef _PHP_TOKYO_TYRANT_PRIVATE_H_
# define _PHP_TOKYO_TYRANT_PRIVATE_H_

#include <tcrdb.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "php_ini.h"
#include "ext/standard/url.h"
#include "SAPI.h" 
#include "php_variables.h"

typedef struct _php_tt_conn {
	TCRDB *rdb; /* database instance */

	zend_bool connected;   /* Are we connected to a db? */
	zend_bool persistent;  /* Is the instant persistent (?) */
} php_tt_conn;

/* {{{ typedef struct _php_tokyo_tyrant_object */
typedef struct _php_tokyo_tyrant_object  {
	
	php_tt_conn *conn; /* database connection */
	zend_object zo;
} php_tokyo_tyrant_object;
/* }}} */

/* {{{ typedef struct _php_tokyo_tyrant_query_object */
typedef struct _php_tokyo_tyrant_query_object  {
	
	php_tt_conn *conn;
	
	RDBQRY *qry;
	zval *parent;
	int pos;
	TCLIST *res;
	
	zend_bool executed;

	zend_object zo;

} php_tokyo_tyrant_query_object;
/* }}} */

/* {{{ typedef struct _php_tokyo_tyrant_iterator_object */
typedef struct _php_tokyo_tyrant_iterator_object  {
	
	php_tt_conn *conn; /* database connection */
	zval *parent;
	
	char *current;
	int current_len;
	
	/* iterating kv or table? */
	int iterator_type;

	zend_object zo;
	
} php_tokyo_tyrant_iterator_object;
/* }}} */

ZEND_BEGIN_MODULE_GLOBALS(tokyo_tyrant)
	HashTable *connections;
	HashTable *failures;

	double default_timeout;
	char *salt;
	char *key_prefix;
	int key_prefix_len;
	
	zend_bool allow_failover;
	long health_check_divisor;
	long fail_threshold;
	
	zend_bool php_expiration;
ZEND_END_MODULE_GLOBALS(tokyo_tyrant)

ZEND_EXTERN_MODULE_GLOBALS(tokyo_tyrant);

#ifdef ZTS
# define TOKYO_G(v) TSRMG(tokyo_tyrant_globals_id, zend_tokyo_tyrant_globals *, v)
#else
# define TOKYO_G(v) (tokyo_tyrant_globals.v)
#endif


#define PHP_TOKYO_CHAIN_METHOD RETURN_ZVAL(getThis(), 1, 0); 

#define PHP_TOKYO_TYRANT_DEFAULT_PORT 1978

#define PHP_TOKYO_TYRANT_OP_PUT        1
#define PHP_TOKYO_TYRANT_OP_PUTKEEP    2
#define PHP_TOKYO_TYRANT_OP_PUTCAT     3
#define PHP_TOKYO_TYRANT_OP_OUT        4
#define PHP_TOKYO_TYRANT_OP_TBLPUT     5
#define PHP_TOKYO_TYRANT_OP_TBLPUTKEEP 6
#define PHP_TOKYO_TYRANT_OP_TBLPUTCAT  7
#define PHP_TOKYO_TYRANT_OP_PUTNR      8
#define PHP_TOKYO_TYRANT_OP_TBLOUT     9

#define PHP_TOKYO_TYRANT_RECTYPE_INT    1
#define PHP_TOKYO_TYRANT_RECTYPE_DOUBLE 2

#define PHP_TOKYO_TYRANT_ITERATOR		1
#define PHP_TOKYO_TYRANT_TABLE_ITERATOR	2

/* {{{ #define PHP_TOKYO_TYRANT_EXCEPTION(intern, format) */
#define PHP_TOKYO_TYRANT_EXCEPTION(intern, format) \
{ \
	int error_code = tcrdbecode(intern->conn->rdb); \
	if (error_code == TTENOREC) { \
		RETURN_NULL(); \
	} \
	zend_throw_exception_ex(php_tokyo_tyrant_exception_sc_entry, error_code TSRMLS_CC, format, tcrdberrmsg(error_code)); \
	return; \
}
/* }}} */

/* {{{ #define PHP_TOKYO_TYRANT_EXCEPTION_FREE(intern, format) */
#define PHP_TOKYO_TYRANT_EXCEPTION_FREE(intern, format) \
{ \
	int error_code = tcrdbecode(intern->conn->rdb); \
	zend_throw_exception_ex(php_tokyo_tyrant_exception_sc_entry, error_code TSRMLS_CC, format, tcrdberrmsg(error_code)); \
	efree(format); \
	return; \
}
/* }}} */

/* {{{ #define PHP_PHP_TOKYO_TYRANT_EXCEPTION_MSG(message) */
#define PHP_TOKYO_TYRANT_EXCEPTION_MSG(message) \
{ \
	zend_throw_exception(php_tokyo_tyrant_exception_sc_entry, message, TTEMISC TSRMLS_CC); \
	return; \
}
/* }}} */

/* {{{ #define PHP_TOKYO_CONNECTED_OBJECT(intern) */
#define PHP_TOKYO_CONNECTED_OBJECT(intern) \
{ \
	intern = (php_tokyo_tyrant_object *)zend_object_store_get_object(getThis() TSRMLS_CC); \
	if (!php_tt_is_connected(intern TSRMLS_CC)) \
		PHP_TOKYO_TYRANT_EXCEPTION_MSG("Not connected to a database"); \
}
/* }}} */

#define DEBUG_M(args...) \
{ \
	char *buf; \
	spprintf(&buf, 1024, args); \
	php_log_err(buf); \
	efree(buf); \
}



#endif /* _PHP_TOKYO_TYRANT_PRIVATE_H_ */