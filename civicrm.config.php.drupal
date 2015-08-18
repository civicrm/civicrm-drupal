<?php

/**
 * This function has been copied from DRUPAL_ROOT/includes/bootstrap.inc
 */

/**
 * Locate the appropriate configuration file.
 *
 * Try finding a matching configuration directory by stripping the
 * website's hostname from left to right and pathname from right to
 * left.  The first configuration file found will be used, the
 * remaining will ignored.  If no configuration file is found,
 * return a default value '$confdir/default'.
 *
 * Example for a fictitious site installed at
 * http://www.drupal.org/mysite/test/ the 'settings.php' is
 * searched in the following directories:
 *
 *  1. $confdir/www.drupal.org.mysite.test
 *  2. $confdir/drupal.org.mysite.test
 *  3. $confdir/org.mysite.test
 *
 *  4. $confdir/www.drupal.org.mysite
 *  5. $confdir/drupal.org.mysite
 *  6. $confdir/org.mysite
 *
 *  7. $confdir/www.drupal.org
 *  8. $confdir/drupal.org
 *  9. $confdir/org
 *
 * 10. $confdir/default
 *
 */

function civicrm_conf_init() {
    global $skipConfigError;

    static $conf = '';

    if ($conf) {
        return $conf;
    }

    // There is much more complex stuff in d7 - but lets just handle sites/default for now....
    $candidates[] = "../../sites/default";
    $candidates[] = "../../../sites/default";

    foreach ($candidates as $candidate) {
      if (is_dir($candidate)) {
        return $candidate;
      }
    }

    throw new Exception(ts('site directory not found'));
}

$settingsFile = civicrm_conf_init() . '/civicrm.settings.php';
define('CIVICRM_SETTINGS_PATH', $settingsFile);
$error = @include_once( $settingsFile );
if ( $error == false ) {
    echo "Could not load the settings file at: {$settingsFile}\n";
    exit( );
}
