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
#ifndef _PHP_TOKYO_TYRANT_SESSION_H_
# define _PHP_TOKYO_TYRANT_SESSION_H_

#include "php_tokyo_tyrant.h"
#include "php_tokyo_tyrant_private.h"
#include "php_tokyo_tyrant_connection.h"
#include "php_tokyo_tyrant_server_pool.h"
#include "php_tokyo_tyrant_failover.h"
#include "php_tokyo_tyrant_funcs.h"

PS_CREATE_SID_FUNC(tokyo_tyrant);

typedef struct _php_tt_session {
	php_tt_server_pool *pool; /* Pool of session servers */
	php_tt_conn *conn;        /* Connection to the session server */
	
	zend_string *pk;           /* Primary key of the session data */
	int   idx;                 /* node where the data is stored at */
	
   zend_string *sess_rand;

	zend_string *checksum;     /* Session validation checksum */
	
	zend_bool remap;    /* If 1 create_sid will remap the session to a new server */

} php_tt_session;

#endif