<?php
// $Id$

function getEntities() {}

/*
 * function to return labels for checkboxes & stuff. Will work for either valid API entites or option valude
 */
function civicrm_rules_get_options($type) {
  if (!civicrm_initialize()) {
    return;
  }
  require_once 'api/api.php';
  //Check if it's an entity - e.g. participant status Type, payment processor, campaign
  $result = civicrm_api($type, 'Get', array('version' => 3));

  if ($result['is_error'] == 1) {
    // it's not an entity - try an option group value
    $result = civicrm_api('OptionValue', 'Get', array('version' => 3, 'option_group_name' => $type));
    foreach ($result['values'] as $options) {
      $values[$options['value']] = $options['label'];
    }
    return $values;
  }
  else {
    //we're using an entity
    foreach ($result['values'] as $key => $fields) {
      $values[$key] = $fields['label'];
    }
    return $values;
  }
}

/**
 * Loads an entity into a version suitable for the rules actions
 */
function _civicrm_rules_load_fromAPI($entity_id, $entity) {
  require_once 'api/api.php';
  $entity = civicrm_api($entity_id, 'Get', array('version' => 3, 'id' => $entity_id));
  return $entity['values'][$entity['id']];
}

