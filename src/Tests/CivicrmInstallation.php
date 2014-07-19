<?php

namespace Drupal\civicrm\Tests;

use Drupal\civicrm\Tests\CivicrmTestBase;

class CivicrmInstallation extends CivicrmTestBase {
  public static function getInfo() {
    return array(
      'name' => 'CiviCRM Installation',
      'description' => 'Tests CiviCRM installation process.',
      'group' => 'CiviCRM',
    );
  }

  public function testCleanInstall() {
    $this->assertTrue(file_exists(conf_path() . '/civicrm.settings.php'), "The civicrm.settings.php file was found in " . conf_path());
    $this->assertTrue(function_exists('civicrm_api3'), 'civicrm_api() function exists.');
    $this->assertNotNull(\CRM_Utils_Type::BIG, "The autoloader has found the \CRM_Utils_Type class.");
  }
}