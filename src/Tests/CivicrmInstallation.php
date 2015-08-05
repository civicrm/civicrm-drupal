<?php

namespace Drupal\civicrm\Tests;

use Drupal\civicrm\Tests\CivicrmTestBase;

/**
 * Tests that CiviCRM installs correctly.
 *
 * @group CiviCRM
 */
class CivicrmInstallation extends CivicrmTestBase {
  public function testCleanInstall() {
    $this->assertTrue(file_exists($this->kernel->getSitePath() . '/civicrm.settings.php'), "The civicrm.settings.php file was found in " . $this->kernel->getSitePath());
    $this->assertTrue(function_exists('civicrm_api3'), 'civicrm_api() function exists.');
    $this->assertNotNull(\CRM_Utils_Type::BIG, "The autoloader has found the \CRM_Utils_Type class.");
  }
}