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
 *
 * civicrm_handler_field_event_type.inc
 * Display's Event Type after grabbing the label from the database
 *
 */
class civicrm_handler_field_encounter_medium extends views_handler_field {
  public static $_encounter_medium;

  public function construct() {
    parent::construct();
    if (!self::$_encounter_medium) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/OptionGroup.php';
      self::$_encounter_medium = CRM_Core_OptionGroup::values('encounter_medium', FALSE);
    }
  }

  public function render($values) {
    $sid = $values->{$this->field_alias};
    if (empty($sid) ||
      (int ) $sid <= 0
    ) {
      return NULL;
    }
    return self::$_encounter_medium[$values->{$this->field_alias}];
  }

}
