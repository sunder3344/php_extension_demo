--TEST--
Check if morse is loaded
--SKIPIF--
<?php
if (!extension_loaded('morse')) {
    echo 'skip';
}
?>
--FILE--
<?php
echo 'The extension "morse" is available';
?>
--EXPECT--
The extension "morse" is available
