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
 * Field handler to provide acess control for an "pseudo constant / option value" pair
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_option extends civicrm_handler_field {
  public static $_options;

  public function render($values) {
    $this->initialize();

    $id = $values->{$this->field_alias};
    if (empty($id) || (int) $id <= 0) {
      return NULL;
    }

    return self::$_options[$values->{$this->field_alias}];
  }

}
