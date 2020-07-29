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
 * Field handler to provide amount raised by given Personal Campaign Page
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_pcp_raised_amount extends views_handler_field {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/PCP/BAO/PCP.php';
    require_once 'CRM/Utils/Money.php';
  }

  public function render($values) {
    // retrieve the value of the "real field"
    $pcp_id = $values->{$this->field_alias};
    // get the total amount raised
    $amount = CRM_PCP_BAO_PCP::thermoMeter($pcp_id);
    if (!$amount) {
      // supply a numeric string if function returns NULL or undesirable result
      $amount = '0';
    }
    return CRM_Utils_Money::format($amount);
  }

}
