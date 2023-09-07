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
 *
 * civicrm_handler_field_pseudo_constant.inc
 * Display's Pseudo Constant after grabbing the label from the database
 *
 */
class civicrm_handler_field_pseudo_constant extends views_handler_field {
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

    // Include and call the Pseudo Class method
    $this->_pseudo_constant = call_user_func_array($this->definition['pseudo class'] . "::" . $this->definition['pseudo method'], $pseudo_args);
  }

  public function render($values) {
    if (!empty($values->{$this->field_alias})) {
      $val = $values->{$this->field_alias};
      // TODO This fix should be replaced as described in  CRM-12853
      $val = str_replace("\x01", '', $val);
      if (isset($this->_pseudo_constant[$val])) {
        return $this->_pseudo_constant[$val];
      }
      return '';
    }
  }

}
