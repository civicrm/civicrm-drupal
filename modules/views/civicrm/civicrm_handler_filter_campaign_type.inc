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
 * CiviCRM Views Integration
 *
 * civicrm_handler_filter_campaign_type.inc
 * Filters Campaign Types, and is configured using the labels from the database
 *
 */
class civicrm_handler_filter_campaign_type extends views_handler_filter_in_operator {
  static $_campaignType;
  function construct() {
    parent::construct();
    if (!self::$_campaignType) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Campaign/PseudoConstant.php';
      self::$_campaignType = CRM_Campaign_PseudoConstant::campaignType();
    }
  }

  function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Campaign Type');
      $options = array();
      foreach (self::$_campaignType as $id => $name) {
        $options[$id] = $name;
      }
      $this->value_options = $options;
    }
  }
}


