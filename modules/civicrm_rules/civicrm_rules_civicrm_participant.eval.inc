<?php
// $Id$

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

