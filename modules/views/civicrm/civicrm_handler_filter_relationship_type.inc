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
 *
 * civicrm_handler_filter_relationship_type.inc
 * Filters Relationship Types, and is configured using the labels from the database
 *
 */
class civicrm_handler_filter_relationship_type extends views_handler_filter_in_operator {
  public static $_relationshipType;

  public function construct() {
    parent::construct();
    if (!self::$_relationshipType) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/PseudoConstant.php';
      $relationshipType_array = CRM_Core_PseudoConstant::relationshipType($columnName = 'label');

      // relationshipType() returns information about relations as array with fields
      // 'label_a_b', 'label_b_a', 'contact_type_a' and 'contact_type_b'.
      // We keep just 'label_a_b' for simplicity.

      $options = array();
      foreach ($relationshipType_array as $id => $value_array) {
        $options[$id] = $value_array['label_a_b'];
      }

      self::$_relationshipType = $options;
    }
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Relationship Type');
      $options = array();
      foreach (self::$_relationshipType as $id => $name) {
        $options[$id] = $name;
      }
      $this->value_options = $options;
    }
  }

}
