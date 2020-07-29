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
 * civicrm_handler_field_relationship_type.inc
 * Displays Relationship Type after grabbing the label from the database
 *
 */
class civicrm_handler_field_relationship_type extends views_handler_field {
  public static $_relationshipType;

  public function construct() {
    parent::construct();
    if (!self::$_relationshipType) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/PseudoConstant.php';
      $relationshipType_array = CRM_Core_PseudoConstant::relationshipType('label');

      // relationshipType() returns information about relations as array with fields
      // 'name_a_b', 'name_b_a', 'contact_type_a' and 'contact_type_b'.
      // We keep just 'label_a_b' for simplicity.

      $options = array();
      foreach ($relationshipType_array as $id => $value_array) {
        $options[$id] = $value_array['label_a_b'];
      }

      self::$_relationshipType = $options;
    }
  }

  public function render($values) {
    $lid = $values->{$this->field_alias};
    if (empty($lid) || (int) $lid <= 0) {
      return NULL;
    }

    return self::$_relationshipType[$values->{$this->field_alias}];
  }

}
