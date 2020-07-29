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
 * Field handler to provide relationship to Drupal User ID
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_drupalid extends views_handler_field {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Core/BAO/UFMatch.php';
  }

  public function render($values) {
    $cid = $values->{$this->field_alias};
    $uf_id = CRM_Core_BAO_UFMatch::getUFId($cid);
    return $uf_id;
  }

}
