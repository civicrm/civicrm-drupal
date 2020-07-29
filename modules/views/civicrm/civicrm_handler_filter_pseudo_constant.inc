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
 * Developed by Jim Taylor.
 * CiviCRM Views Integration
 *
 * civicrm_handler_filter_activity_status.inc
 * Filters Activity Statuses, and is configured using the labels from the database
 *
 */
class civicrm_handler_filter_pseudo_constant extends views_handler_filter_in_operator {
  public $_pseudo_constant;

  public function construct() {
    parent::construct();
    if (!civicrm_initialize() ||
      !isset($this->definition['pseudo class']) ||
      !isset($this->definition['pseudo method'])
    ) {
      return;
    }

    // Load pseudo arguments if passed as array from hook_views_data
    if (isset($this->definition['pseudo args']) && is_array($this->definition['pseudo args'])) {
      $pseudo_args = $this->definition['pseudo args'];
    }
    elseif (isset($this->definition['dao class']) && isset($this->definition['real field'])) {
      $pseudo_args = array($this->definition['dao class'], $this->definition['real field']);
    }
    else {
      $pseudo_args = array();
    }

    require_once str_replace('_', DIRECTORY_SEPARATOR, $this->definition['pseudo class']) . '.php';
    // Include and call the Pseudo Class method
    $this->_pseudo_constant = call_user_func_array($this->definition['pseudo class'] . "::" . $this->definition['pseudo method'], $pseudo_args);
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = $this->definition['title'];
      $options = array();
      foreach ($this->_pseudo_constant as $id => $name) {
        $options[$id] = $name;
      }
      $this->value_options = $options;
    }
  }

}
