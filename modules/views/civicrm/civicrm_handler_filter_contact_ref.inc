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
 * Filters Contact reference custom field.
 */
class civicrm_handler_filter_contact_ref extends views_handler_filter_string {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
  }

  public function op_equal($field) {
    $this->filterContactRef($field);
  }

  public function op_contains($field) {
    $this->filterContactRef($field);
  }

  /**
   * Adds where clause to the view query to filter
   * by contact sort_name instead of id.
   *
   * @param string $field
   */
  public function filterContactRef($field) {
    if (!empty($this->value)) {
      $op = $this->operator;
      if ($this->operator != '=' && $this->operator != '!=') {
        $op = 'LIKE';
        $this->value = "%{$this->value}%";
      }
      $contacts = db_select('civicrm_contact', 'cc')
        ->fields('cc', array('id'))
        ->condition('cc.sort_name', $this->value, $op);

      $this->query->add_where($this->options['group'], $field, $contacts, 'IN');
    }
  }

}
