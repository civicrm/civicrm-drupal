<?php

namespace CiviDrupal;

use Civi\Test\EndToEndInterface;

class PhpVersionTest extends \PHPUnit\Framework\TestCase implements EndToEndInterface {

  /**
   * CIVICRM_DRUPAL_PHP_MINIMUM (civicrm.module) should match
   * CRM_Upgrade_Incremental_General::MIN_INSTALL_PHP_VER.
   */
  public function testConstantMatch(): void {
    $constantFile = $this->getDrupalModulePath() . '/civicrm.module';
    $this->assertFileExists($constantFile);
    $content = file_get_contents($constantFile);
    if (preg_match(";define\\('CIVICRM_DRUPAL_PHP_MINIMUM', '(.*)'\\);", $content, $m)) {
      $a = preg_replace(';^(\d+\.\d+(?:\.[1-9]\d*)?).*$;', '\1', \CRM_Upgrade_Incremental_General::MIN_INSTALL_PHP_VER);
      $b = preg_replace(';^(\d+\.\d+(?:\.[1-9]\d*)?).*$;', '\1', $m[1]);
      $this->assertEquals($a, $b);
    }
    else {
      $this->fail('Failed to find CIVICRM_DRUPAL_PHP_MINIMUM in ' . $constantFile);
    }
  }

  /**
   * "php" requirement (civicrm.info) should match
   * CRM_Upgrade_Incremental_General::MIN_INSTALL_PHP_VER.
   */
  public function testInfoMatch(): void {
    $infoFile = $this->getDrupalModulePath() . '/civicrm.info';
    $this->assertFileExists($infoFile);
    $info = drupal_parse_info_file($infoFile);
    $expectMajorMinor = preg_replace(';^(\d+\.\d+(?:\.[1-9]\d*)?).*$;', '\1', \CRM_Upgrade_Incremental_General::MIN_INSTALL_PHP_VER);
    $this->assertEquals($expectMajorMinor, $info['php']);
  }

  /**
   * @return string
   *   Ex: '/var/www/sites/all/modules/civicrm/drupal'
   */
  protected function getDrupalModulePath() {
    return dirname(dirname(dirname(__DIR__)));
  }

}
