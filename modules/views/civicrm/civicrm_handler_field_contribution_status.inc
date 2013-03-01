<?php
// $Id$

/*
 +--------------------------------------------------------------------+
 | CiviCRM version 3.4                                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/

/*
 * Heavily based on anonymous code contribution. No claim.
 *
 * civicrm_handler_field_contribution_status.inc
 * Displays Contribution Status after grabbing the label from the database
 *
 */
class civicrm_handler_field_contribution_status extends views_handler_field {
  static $_contributionStatus;
  function construct() {
    if (!self::$_contributionStatus) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Contribute/PseudoConstant.php';
      self::$_contributionStatus = CRM_Contribute_PseudoConstant::contributionStatus();
    }
  }

  function render($values) {
    $lid = $values->{$this->field_alias};
    if (empty($lid) ||
      (int ) $lid <= 0
    ) {
      return NULL;
    }

    return self::$_contributionStatus[$values->{$this->field_alias}];
  }
}


