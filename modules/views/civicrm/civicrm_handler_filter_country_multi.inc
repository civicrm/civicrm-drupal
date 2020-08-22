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
 * @file Provides Views integration for CiviCRM Multi-Select Country custom fields
 *
 * @author DaveJ
 * based on civicrm_handler_filter_custom_option and civicrm_handler_filter_country
 */
class civicrm_handler_filter_country_multi extends views_handler_filter_in_operator {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      $countries = CRM_Core_PseudoConstant::country();
      $options = array();
      if (is_array($countries)) {
        foreach ($countries as $id => $name) {
          $options[$id] = $name;
        }
      }
      $this->value_options = $options;
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

    // negated operator uses AND, positive uses OR
    $op = $glue = NULL;
    switch ($this->operator) {
      case 'in':
        $op = ' LIKE ';
        $glue = ' OR ';
        break;

      case 'not in':
        $op = ' NOT LIKE ';
        $glue = ' AND ';
        break;

      case 'all':
        $clause = "$this->table_alias.$this->real_field IN (" . implode(',', $this->value) . ")";
        break;
    }

    if ($op) {
      foreach ($this->value as $value) {
        $clauses[] = "$this->table_alias.$this->real_field " . $op . "'%" . $value . "%'";
      }
      $clause = implode($glue, $clauses);
    }

    $this->query->add_where_expression($this->options['group'], $clause);
  }

}
