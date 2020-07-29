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
 * Field handler to provide a human-readable CiviCRM component name
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_component extends views_handler_field {

  public function render($values) {
    $value = $this->get_value($values);
    return CRM_Core_Component::getComponentName($value);
  }

}
