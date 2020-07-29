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
 * Heavily based on anonymous code contribution. No claim.
 * CiviCRM Views Integration
 */
class civicrm_handler_filter_group_status extends views_handler_filter_in_operator {

  public static $groupContactStatus = NULL;

  public function construct() {
    parent::construct();

    if (!self::$groupContactStatus) {
      if (!civicrm_initialize()) {
        return;
      }
      self::$groupContactStatus = array(
        0 => 'Added',
        1 => 'Removed',
        2 => 'Pending',
      );
    }
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Contact Group Status');
      $options = array();
      foreach (self::$groupContactStatus as $id => $name) {
        $options[$name] = $name;
      }
      $this->value_options = $options;
    }
  }

}
