<?php

namespace Drupal\civicrm_views\Tests;

use Drupal\civicrm\Tests\CivicrmTestBase;
use Drupal\views\Views;

/**
 * Tests basic CiviCRM views functionality.
 *
 * @group CiviCRM
 */
class CivicrmViewsTest extends CivicrmTestBase {
  public static $modules = array('civicrm_views', 'civicrm_views_config');
  // @Todo: Provide schema declaraction
  protected $strictConfigSchema = FALSE;

  protected $contact_data = array(
    array(
      'contact_type' => 'Individual',
      'first_name' => 'John',
      'last_name' => 'Smith',
      'api.email.create' => array(
        array(
          'email' => 'john.smith@example.com',
          'is_primary' => TRUE,
        ),
      ),
      'api.address.create' => array(
        array(
          'street_address' => '14 Main Street',
          'is_primary' => TRUE,
          'location_type_id' => 'Home',
        ),
      ),
      'api.entity_tag.create' => array(
        'tag_id' => 'Volunteer',
      ),
      'api.relationship.create' => array(
        'relationship_type_id' => 5, // Employee of
        'contact_id_a' => '$value.id',
        'contact_id_b' => 1, // Default Organization
      ),
    ),
    array(
      'contact_type' => 'Individual',
      'first_name' => 'Jane',
      'last_name' => 'Smith',
      'api.email.create' => array(
        array(
          'email' => 'jane.smith@example.com',
          'is_primary' => TRUE,
        ),
        array(
          'email' => 'jane.smithy@example.com',
        ),
      ),
      'api.address.create' => array(
        array(
          'street_address' => '3 Broadway Avenue',
          'is_primary' => TRUE,
          'location_type_id' => 'Work',
        ),
        array(
          'street_address' => '5 Garden Grove',
          'location_type_id' => 'Home',
        ),
      ),
      'api.entity_tag.create' => array(
        'tag_id' => 'Company',
      ),
      'api.relationship.create' => array(
        'relationship_type_id' => 5, // Employee of
        'contact_id_a' => '$value.id',
        'contact_id_b' => 1, // Default Organization
      ),
    ),
  );

  protected function createData() {
    foreach ($this->contact_data as $contact) {
      civicrm_api3('Contact', 'create', $contact);
    }

    $result = civicrm_api3('Contact', 'get', array(
      'options' => array('limit' => 100),
      'api.email.get' => 1,
      'api.entity_tag.get' => 1,
      'api.address.get' => 1,
    ));

    $this->assertTrue(empty($result['is_error']), "api.contact.get result OK.");
    $this->assertEqual(3, count($result['values']), "3 contacts have been created.");
    $this->verbose("<pre>" . var_export($result, TRUE) . "</pre>");
  }

  public function testCivicrmViewsTest() {
    $this->createData();

    $render = array(
      'view' => array(
        '#type' => 'view',
        '#name' => 'contacts',
        '#display_id' => 'default',
        '#arguments' => array(),
      ),
    );

    // @Todo: Why do we need to call this?
    $view = Views::getView('contacts');
    $this->dieOnFail = TRUE;
    $this->assertTrue(is_object($view), "View object loaded.");
    $this->dieOnFail = FALSE;

    $output = $view->preview();
    $output = \Drupal::service('renderer')->render($output, TRUE);
    $this->setRawContent($output);

    $xpath = $this->xpath('//div[@class="view-content"]');
    $this->assertTrue($xpath, 'View content has been found in the rendered output.');

    $this->verbose($this->getRawContent());

    $xpath = $this->xpath('//tbody/tr');
    $this->assertEqual(3, count($xpath), "There are 3 rows in the table.");

    foreach ($xpath as $key => $tr) {
      if ($key == 0) continue; // Skip Default Organization

      $this->assertEqual("{$this->contact_data[$key - 1]['first_name']} {$this->contact_data[$key - 1]['last_name']}", trim((string) $tr->td[1]));
      $this->assertEqual($this->contact_data[$key - 1]['api.email.create'][0]['email'], trim((string) $tr->td[2]));
      $this->assertEqual($this->contact_data[$key - 1]['api.address.create'][0]['street_address'], trim((string) $tr->td[3]));
      $this->assertEqual($this->contact_data[$key - 1]['api.entity_tag.create']['tag_id'], trim((string) $tr->td[4]));
      $this->assertEqual('Default Organization', trim((string) $tr->td[5]));
    }
  }
}