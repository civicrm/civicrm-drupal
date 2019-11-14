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
 * Handler to get the user. This will work if $entityobj->contact_id is defined
 * e.g. for participant object, contribution object
 */
function civicrm_rules_events_argument_civicrm_contactID_load_user(&$entityobj) {
  $contact = civicrm_api('UFMatch', 'get', array('version' => 3, 'contact_id' => $entityobj->contact_id, 'sequential' => 1));
  if (is_numeric($contact['values'][0]['uf_id'])) {
    return user_load(array('uid' => $contact['values'][0]['uf_id']));
  }
}

