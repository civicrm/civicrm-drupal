<?php
// $Id$

/*
 +--------------------------------------------------------------------+
 | CiviCRM version 3.4                                                |
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC (c) 2004-2011                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007.                                       |
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

/**
 * Condition civicrm_participant_role - return true if it matches chosen role
 */
function civicrm_rules_condition_participant_role(&$entityobj, $settings) {
  return empty($settings['civicrm_participant_role'][$entityobj->role_id]) ? FALSE : TRUE;
}

/*
 * *
 * Condition civicrm_event status
 */
function civicrm_rules_condition_participant_status(&$entityobj, &$settings) {
  return empty($settings['civicrm_participant_status'][$entityobj->status_id]) ? FALSE : TRUE;
}

/**
 * Condition: Check for content types - Configuration form
 */
function civicrm_rules_condition_participant_status_form($settings, &$form) {
  $form['settings']['civicrm_participant_status'] = array(
    '#type' => 'checkboxes',
    '#title' => t('CiviCRM Participant Status'),
    '#options' => civicrm_rules_get_options('participant_status_type'),
    '#multiple' => TRUE,
    '#default_value' => isset($settings['civicrm_participant_status']) ? $settings['civicrm_participant_status'] : array(),
    '#required' => TRUE,
  );
}

/**
 * Condition: Check for content types - Configuration form
 */
function civicrm_rules_condition_participant_role_form($settings, &$form) {
  $form['settings']['civicrm_participant_role'] = array(
    '#type' => 'checkboxes',
    '#title' => t('CiviCRM Participant Role'),
    '#options' => civicrm_rules_get_options('participant_role'),
    '#multiple' => TRUE,
    '#default_value' => isset($settings['civicrm_participant_role']) ? $settings['civicrm_participant_role'] : array(),
    '#required' => TRUE,
  );
}

