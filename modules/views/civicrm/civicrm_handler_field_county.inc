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
 * civicrm_handler_field_county.inc
 * Displays County after grabbing the label from the database
 *
 */
class civicrm_handler_field_county extends civicrm_handler_field_address {
  public static $_counties;

  public function construct() {
    parent::construct();
    if (!self::$_counties) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/PseudoConstant.php';
      self::$_counties = CRM_Core_PseudoConstant::county();
    }
  }

  public function render($values) {
    $sid = $values->{$this->field_alias};
    if (empty($sid) ||
      (int ) $sid <= 0
    ) {
      return NULL;
    }

    return self::$_counties[$values->{$this->field_alias}];
  }

  public function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
  }

}
