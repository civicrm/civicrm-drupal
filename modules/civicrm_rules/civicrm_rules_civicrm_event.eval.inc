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
 * Condition: Check for content types - Configuration form
 */
function civicrm_rules_condition_event_type_form($settings, &$form) {
  $form['settings']['civicrm_event_type'] = array(
    '#type' => 'checkboxes',
    '#title' => t('CiviCRM Event Types'),
    '#options' => civicrm_rules_get_options('event_type'),
    '#multiple' => TRUE,
    '#default_value' => isset($settings['civicrm_event_type']) ? $settings['civicrm_event_type'] : array(),
    '#required' => TRUE,
  );
}

/*
 * Get Node with cck field ending in civicrm_event = event
 * Ideally this would be extended to a generic linkage between CCK & Civi entities
 */
function civicrm_rules_condition_event_node(&$entityobj) {

  $id = $entityobj->event_id;
  if (is_a($entityobj, 'CRM_Event_DAO_Event') || is_a($entityobj, 'CRM_Event_BAO_Event')) {
    $id = $entityobj->id;
  }

  $sql = "SELECT * FROM `content_type_og_node` WHERE `field_civicrm_event_value` = %d";
  $nid = db_result(db_query($sql, $id));
  return $id == $nid ? TRUE : FALSE;
}



/*
 * *
 * Condition civicrm_event type
 */
function civicrm_rules_condition_event_type(&$entityobj, &$settings) {
  return empty($settings['civicrm_event_type'][$entityobj->event_type_id]) ? FALSE : TRUE;
}

/**
 * Handler to get the user.
 * @todo this should either move into an event specific file or be more generic
 */
function civicrm_rules_events_argument_civicrm_event(&$eventobj) {
  static $result;
  static $contact;
  if (empty($result) || empty($contact)) {
    //todo BAO POST hook leaves created_id blank, the api call is a work-around for now
    if (empty($eventobj->created_by)) {
      require_once 'api/api.php';
      $result = civicrm_api('Event', 'get', array('version' => 3, 'id' => $eventobj->id, 'sequential' => 1));
      $contact = civicrm_api('UFMatch', 'get', array('version' => 3, 'contact_id' => $result['values'][0]['created_id'], 'sequential' => 1));
    }
  }

  if (is_numeric($contact['values'][0]['uf_id'])) {
    return user_load(array('uid' => $contact['values'][0]['uf_id']));
  }
}

/*
 *get node related via cck  - this relies on there being a field called civicrm_event.
 *If you use this field on more than one event type it will load the last one (not something you should do)
 *I wanted to do it using a civicrm_event cck type but got a bit stuck.
 *Gets node related to event if passed an event or participant object
 */
function civicrm_rules_events_argument_civicrm_eventnode(&$entityobj) {

  if (is_a($entityobj, 'CRM_Event_BAO_Event') || is_a($entityobj, 'CRM_Event_DAO_Event')) {
    $eventID = $entityobj->id;
  }

  if (is_a($entityobj, 'CRM_Event_BAO_Participant') || is_a($entityobj, 'CRM_Event_DAO_Participant')) {
    $eventID = $entityobj->event_id;
  }

  //todo there must be a clever way to get a node based on the cck field but I don't know it yet
  //I looked into the search_index_links but that seems to rely on touching the node
  // really I am only needing type_name now as I am specifying the field name to be
  // field_civicrm_event_value but leaving this in in case I go back to the cck way
  $sql = "SELECT nf.field_name, i.type_name
FROM {content_node_field} nf
LEFT JOIN {content_node_field_instance} i ON nf.field_name = i.field_name
WHERE nf.field_name LIKE '%civicrm_event'
LIMIT 1";


  $results = db_query($sql);

  while ($result = db_fetch_object($results)) {
    //todo figure out why cck field ends in contact_id
    $sql = "SELECT nid  FROM {content_type_" . $result->type_name . "} WHERE " . $result->field_name . "_value = %d";
    $nodeid = db_result(db_query($sql, $eventID));
    return $node = node_load(array('nid' => $nodeid));
  }
}

