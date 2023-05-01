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
   * @param string $field
   */
  public function op_contact_id($field) {
    $this->operator = '=';
    $this->filterContactRef($field, 'id');
  }

  /**
   * Add Contact ID to the list of operators.
   */
  public function operators() {
    $op = parent::operators();
    $op['contact_id'] = [
      'title' => t('Contact ID'),
      'short' => t('contact_id'),
      'method' => 'op_contact_id',
      'values' => 1,
    ];
    return $op;
  }

  /**
   * Adds where clause to the view query to filter
   * by contact sort_name OR id based on operator selected.
   *
   * @param string $field
   * @param string $fieldName
   */
  public function filterContactRef($field, $fieldName = 'sort_name') {
    if (!empty($this->value)) {
      $op = $this->operator;
      if ($this->operator != '=' && $this->operator != '!=') {
        $op = 'LIKE';
        $this->value = "%{$this->value}%";
      }
      $contacts = db_select('civicrm_contact', 'cc')
        ->fields('cc', array('id'))
        ->condition("cc.{$fieldName}", $this->value, $op);

      $this->query->add_where($this->options['group'], $field, $contacts, 'IN');
    }
  }

}
