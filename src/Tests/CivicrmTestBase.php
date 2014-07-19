<?php

/**
 * @file
 * Definition of Drupal\civicrm\Tests\CivicrmTestBase
 *
 * This class allows CiviCRM to run inside SimpleTest. The main issue we face is that CiviCRM
 * doesn't allow for table prefixing — which is how Drupal creates a test installation
 * in the same database. So in this class we attempt to create a new database, which is defined
 * in settings.php as the 'civicrm_test' database, which civicrm will then use in the course of
 * the test. It is deleted afterwards as part of the tear down.
 */

namespace Drupal\civicrm\Tests;

use Drupal\simpletest\WebTestBase;
use Drupal\Core\Database\Database;

abstract class CivicrmTestBase extends WebTestBase {
  public static $modules = array('civicrm');

  protected function setUp() {
    // If the civicrm_test database already exists, first drop it.
    try {
      $conn = Database::getConnection('default', 'civicrm_test');
      $database = $conn->getConnectionOptions()['database'];
      $conn->query("DROP DATABASE $database"); // Todo: get this working when db name passed in as an argument
      $conn->destroy();
    }
    catch (\Exception $e) {
      // Pass.
    }

    // Now attempt to create the database.
    // This method is taken from Drupal\Core\Database\Driver\mysql\Install\Tasks.

    // Remove the database string from connection info.
    $connection_info = Database::getConnectionInfo('civicrm_test');
    $database = $connection_info['default']['database'];
    unset($connection_info['default']['database']);

    // In order to change the Database::$databaseInfo array, need to remove
    // the active connection, then re-add it with the new info.
    Database::removeConnection('civicrm_test');
    Database::addConnectionInfo('civicrm_test', 'default', $connection_info['default']);

    // Now, attempt the connection again; if it's successful, attempt to
    // create the database.
    Database::getConnection('civicrm_test')->createDatabase($database);
    if (!$connection_info) {
      throw new \RuntimeException("No default connection info!");
    }

    // Now add the civicrm_test connection info back *with* the database key present.
    $connection_info['default']['database'] = $database;
    Database::removeConnection('civicrm_test');
    Database::addconnectionInfo('civicrm_test', 'default', $connection_info['default']);

    parent::setUp();
  }

  protected function tearDown() {
    $conn = Database::getConnection('default', 'civicrm_test');
    $database = $conn->getConnectionOptions()['database'];
    $conn->query("DROP DATABASE $database"); // Todo: get this working when db name passed in as an argument
    $conn->destroy();
  }
}