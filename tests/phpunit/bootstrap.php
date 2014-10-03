<?php

if (getenv('DRUPAL_ROOT')) {
  define('DRUPAL_ROOT', getenv('DRUPAL_ROOT'));
}
else {
  // drupal/sites/all/modules/civicrm/drupal/tests/phpunit
  define('DRUPAL_ROOT', dirname(dirname(dirname(dirname(dirname(dirname(dirname(realpath(__DIR__)))))))));
}

// Argh.  Crib from _drush_bootstrap_drupal_site_validate
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';

chdir(DRUPAL_ROOT);
require_once('includes/bootstrap.inc');
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// Drupal just usurped PHPUnit's error handler.  Kick it off the throne.
restore_error_handler();

civicrm_initialize();
