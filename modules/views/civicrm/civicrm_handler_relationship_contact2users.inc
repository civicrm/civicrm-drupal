<?php
/*
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC. All rights reserved.                        |
 |                                                                    |
 | This work is published under the GNU AGPLv3 license with some      |
 | permitted exceptions and without any warranty. For full license    |
 | and copyright information, see https://civicrm.org/licensing       |
 +--------------------------------------------------------------------+
 */

/**
 * Field handler to provide relationship to Drupal User ID
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_relationship_contact2users extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  public function query() {

    $join = new views_join();
    $join->definition = array(
      'left_table' => 'civicrm_contact',
      'left_field' => 'id',
      'table' => 'civicrm_uf_match',
      'field' => 'contact_id',
    );

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    // Continue our JOIN
    $join->construct();

    $this->first_join = $this->query->add_table('civicrm_uf_match', $this->relationship, $join);

    // Then, create a relationship on that table:
    $join = new views_join();
    $join->definition = array(
      'left_table' => $this->first_join,
      'left_field' => 'uf_id',
      'table' => 'users',
      'field' => 'uid',
    );

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    $join->construct();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];

    $this->alias = $this->query->add_relationship($alias, $join, 'users');
  }

}
