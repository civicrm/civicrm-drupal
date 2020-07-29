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
 * Field handler to provide relationship between memberships and contributions
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_relationship_memberships_contributions extends views_handler_relationship {

  /**
   * Called to implement a relationship in a query.
   */
  public function query() {
    $join = new views_join();
    if ($this->options['table'] == "civicrm_membership") {
      $join->definition = array(
        'left_table' => 'civicrm_membership',
        'left_field' => 'id',
        'table' => 'civicrm_membership_payment',
        'field' => 'membership_id',
      );
    }
    else {
      $join->definition = array(
        'left_table' => 'civicrm_contribution',
        'left_field' => 'id',
        'table' => 'civicrm_membership_payment',
        'field' => 'contribution_id',
      );

    }

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    // Continue our JOIN
    $join->construct();

    $this->first_join = $this->query->add_table('civicrm_membership_payment', $this->relationship, $join);

    // Then, create a relationship on that table:
    $join = new views_join();
    $join->definition = array(
      'left_table' => $this->first_join,
      'left_field' => $this->options['field'],
      'table' => $this->definition['base'],
      'field' => $this->definition['base field'],
    );

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    $join->construct();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];

    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }

}
