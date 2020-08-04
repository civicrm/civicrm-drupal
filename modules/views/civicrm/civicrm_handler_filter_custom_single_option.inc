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
  * @file Provides Views integration for custom CiviCRM custom field option groups
  *
  * @author Matt Chapman <Matt@NinjitsuWeb.com>
  */
class civicrm_handler_filter_custom_single_option extends views_handler_filter_in_operator {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      $customFieldID = CRM_Core_DAO::getFieldValue('CRM_Core_BAO_CustomField', $this->real_field, 'id', 'column_name');
      $options = (array) CRM_Core_BAO_CustomOption::getCustomOption($customFieldID);

      foreach ($options as $id => $opt) {
        $this->value_options[$opt['value']] = strip_tags($opt['label']);
      }
    }
  }

}
