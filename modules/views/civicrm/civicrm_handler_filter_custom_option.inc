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
 * @file Provides Views integration for multi valued fields like
 * contact subtype and CiviCRM custom field option groups
 *
 * @author Matt Chapman <Matt@NinjitsuWeb.com>
 */
class civicrm_handler_filter_custom_option extends views_handler_filter_in_operator {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      if ($this->real_field == 'contact_sub_type') {
        require_once 'CRM/Contact/BAO/ContactType.php';
        $this->value_options = CRM_Contact_BAO_ContactType::subTypePairs();
      }
      else {
        $customFieldID = CRM_Core_DAO::getFieldValue('CRM_Core_BAO_CustomField', $this->real_field, 'id', 'column_name');
        $options = (array) CRM_Core_BAO_CustomOption::getCustomOption($customFieldID);
        foreach ($options as $id => $opt) {
          $this->value_options[$opt['value']] = strip_tags($opt['label']);
        }
      }
    }
  }

  public function operators() {
    $operators = parent::operators();
    $operators += array(
      'all' => array(
        'title' => t('Is all of'),
        'short' => t('all'),
        'method' => 'op_simple',
        'values' => 1,
      ),
    );

    return $operators;
  }

  public function op_simple() {
    if (empty($this->value)) {
      return;
    }

    $this->ensure_my_table();

    $sep = CRM_Core_DAO::VALUE_SEPARATOR;

    // negated operator uses AND, positive uses OR
    $op = ($this->operator == 'in' || $this->operator == 'all') ? 'LIKE' : 'NOT LIKE';
    $glue = ($this->operator == 'in') ? 'OR ' : 'AND ';
    foreach ($this->value as $value) {
      $clauses[] = "$this->table_alias.$this->real_field " . $op . " '%" . $sep . CRM_Core_DAO::escapeString($value) . $sep . "%' ";
    }
    $clause = implode($glue, $clauses);
    $this->query->add_where_expression($this->options['group'], $clause);
  }

}
