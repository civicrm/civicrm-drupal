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
 * CiviCRM handler for options with the in operator.
 */
class civicrm_handler_filter_option extends views_handler_filter_in_operator {
  public static $_options;

  public function get_value_options() {
    $this->initialize();

    if (!isset($this->value_options)) {
      $this->value_title = $this->title();
      $options = array();
      foreach (self::$_options as $id => $name) {
        $options[$id] = $name;
      }
      $this->value_options = $options;
    }
  }

}
