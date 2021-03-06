--TEST--
Test string with null char in the middle of key
--SKIPIF--
<?php
include dirname(__FILE__) . "/skipif.inc";
?>
--FILE--
<?php
include dirname(__FILE__) . '/config.inc';

$tt = new TokyoTyrant(TT_HOST, TT_PORT);
$tt->vanish();

$key = 'START' . "\0" . 'END';
$var = 'test data'; 

$tt->put($key, $var);

/* This should fail */
var_dump($tt->get("START"));
var_dump($tt->get($key));

$tt->vanish();

/* test put shl */
$tt->put($key, "abc"); 
$tt->putshl($key, "de", 4); 
var_dump($tt->get($key));

$tt = new TokyoTyrant(TT_HOST, TT_PORT);
$tt->vanish();

ini_set("tokyo_tyrant.key_prefix", "my_prefix_");
$tt->put($key, $var);

var_dump($tt->get("START"));
var_dump($tt->get($key));

$tt->vanish();

ini_set("tokyo_tyrant.key_prefix", "my\0prefix");
$tt->put($key, $var);

ini_set("tokyo_tyrant.key_prefix", null);
$tt->put("my", "wrong data");
var_dump($tt->get("my\0prefix" . $key));


?>
--EXPECT--
NULL
string(9) "test data"
string(4) "bcde"
NULL
string(9) "test data"
string(9) "test data"