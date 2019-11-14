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
 *
 * @package CRM
 * @copyright CiviCRM LLC https://civicrm.org/licensing
 * $Id$
 *
 */

/**
 * This file contains functions that calls civicrm apis for data manupulation / retrieval
 */

/**
 * Function to get the list of groups
 */
function _civicrm_get_groups() {
  civicrm_initialize();
  $groupInformation = civicrm_api('group', 'get', array('version' => 3));

  $groups = array();
  foreach ($groupInformation['values'] as $values) {
    $groups[$values['id']] = $values['title'];
  }

  return $groups;
}

/**
 * Function to add contacts to group
 */
function _civicrm_rules_group_contact($contactId, $groupId, $action = 'create') {
  civicrm_initialize();
  $params = array(
    'contact_id' => $contactId,
    'group_id' => $groupId,
    'version' => 3,
  );
  // This used to be civicrm_group_contact_common($params, $action);
  civicrm_api('group_contact', $action, $params);
}

/**
 * Loads a contact into a version suitable for the rules actions
 */
function _civicrm_rules_load_contact($contactID) {
  $contact = new CRM_Contact_DAO_Contact();
  $contact->id = $contactID;

  if ($contact->id) {
    // load the contact data here
    $contact->find(TRUE);
  }

  return $contact;
}

function _civicrm_rules_in_group($contact, $group) {
  // check if contact is in the group gid
  return TRUE;
}

/**
 * Loads a mailing into a version suitable for the rules actions
 */
function _civicrm_rules_load_mailing($mailingID) {
  $mailing = new CRM_Mailing_DAO_Mailing();
  $mailing->id = $mailingID;

  if ($mailing->id) {
    // load the mailing data here
    $mailing->find(TRUE);
  }

  return $mailing;
}

