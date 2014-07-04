<?php

function civicrm_views_views_data() {
  try {
    \Drupal::service('civicrm');
  }
  catch (Exception $e) {
    \Drupal::logger('civicrm_views')->error("Unable to initialize CiviCRM. Aborting hook_views_data().");
    return array();
  }

  // @Todo Activities, Addresses, Emails, Phones, Contributions, Tags
  $civicrm_entities = array(
    'civicrm_contact' => array(
      'display_name' => t('CiviCRM Contact'),
      'dao' => 'CRM_Contact_DAO_Contact',
      'base' => TRUE,
    ),
    'civicrm_event' => array(
      'display_name' => 'CiviCRM Event',
      'dao' => 'CRM_Event_DAO_Event',
      'base' => TRUE,
    ),
    'civicrm_group' => array(
      'display_name' => 'CiviCRM Group',
      'dao' => 'CRM_Contact_DAO_Group',
      'base' => TRUE,
    ),
    'civicrm_group_contact' => array(
      'display_name' => 'CiviCRM Group Contact',
      'dao' => 'CRM_Contact_DAO_GroupContact',
      'base' => FALSE,
    ),
    'civicrm_membership' => array(
      'display_name' => 'CiviCRM Membership',
      'dao' => 'CRM_Member_DAO_Membership',
      'base' => TRUE,
    ),
    'civicrm_membership_type' => array(
      'display_name' => 'CiviCRM Membership Type',
      'dao' => 'CRM_Member_DAO_MembershipType',
      'base' => FALSE,
    ),
    'civicrm_participant' => array(
      'display_name' => 'CiviCRM Participant',
      'dao' => 'CRM_Event_BAO_Participant',
      'base' => FALSE,
    ),
    'civicrm_relationship' => array(
      'display_name' => 'CiviCRM Relationship',
      'dao' => 'CRM_Contact_DAO_Relationship',
      'base' => TRUE,
    ),
    'civicrm_relationship_type' => array(
      'display_name' => 'CiviCRM Relationship Type',
      'dao' => 'CRM_Contact_DAO_RelationshipType',
      'base' => FALSE,
    ),
  );

  $data = array();
  foreach ($civicrm_entities as $table => $civicrm_entity) {
    $data[$table]['table']['group'] = $civicrm_entity['display_name'];

    if ($civicrm_entity['base']) {
      $data[$table]['table']['base'] = array(
        'field' => 'id',
        'title' => $civicrm_entity['display_name'],
        //'database' => 'civicrm',
      );
    };

    $fields = $civicrm_entity['dao']::fields();
    ksm($fields);
    foreach ($fields as $field) {
      $data[$table][$field['name']] = array(
        'title' => $field['title'] ? $field['title'] : "{$civicrm_entity['name']}.{$field['name']}",
        'help' => $field['title'] ? $field['title'] : "{$civicrm_entity['name']}.{$field['name']}",
        'field' => _civicrm_views_field_to_field_plugin($civicrm_entity, $field),
        'sort' => array(
          'id' => _civicrm_views_type_to_sort_plugin($field['type']),
        ),
        'filter' => _civicrm_views_type_to_filter_plugin($civicrm_entity, $field),
        'relationship' => _civicrm_views_field_to_relationship_plugin($civicrm_entity, $field),
      );
    }
  }

  // Explicit relationships
  $data['civicrm_group']['id_group_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_group_contact',
      'base field' => 'group_id',
      'title' => t('CiviCRM Group Contact'),
      'label' => t('CiviCRM Group Contact (from Group.id)'),
      'help' => t('CiviCRM Group Contact (from Group.id)'),
    ),
  );
  $data['civicrm_event']['id_participant'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_participant',
      'base field' => 'event_id',
      'title' => t('CiviCRM Participant'),
      'label' => t('CiviCRM Participant (from Participant.id)'),
      'help' => t('CiviCRM Participant (from Participant.id)'),
    ),
  );
  $data['civicrm_relationship_type']['id_relationship'] = array(
    'id' => 'standard',
    'field' => 'id',
    'base' => 'civicrm_relationship',
    'base field' => 'relationship_type_id',
    'title' => t('CiviCRM Relationship'),
    'label' => t('CiviCRM Relationship (from Relationship.id)'),
    'help' => t('CiviCRM Relationship (from Relationship.id)'),
  );

  // @Todo: other explicit relationships required
  // contacts => group_contacts
  // contacts => memberships
  // contacts => relationships
  // contacts => participants
  // implicit join: participant to contact
  // implicit join: participant to event
  // implicit join: membership to membership type

  ksm($data);

  return $data;
}

/**
 * T_INT        = 1,
 * T_STRING     = 2,
 * T_ENUM       = 2,
 * T_DATE       = 4,
 * T_TIME       = 8,
 * T_BOOLEAN    = 16,
 * T_TEXT       = 32,
 * T_LONGTEXT   = 32,
 * T_BLOB       = 64,
 * T_TIMESTAMP  = 256,
 * T_FLOAT      = 512,
 * T_MONEY      = 1024,
 * T_EMAIL      = 2048,
 * T_URL        = 4096,
 * T_CCNUM      = 8192,
 * T_MEDIUMBLOB = 16384;
 *
 * @Todo: display currencies nicely
 */
function _civicrm_views_field_to_field_plugin($entity, $field) {
  Drupal::service('civicrm');

  if (!empty($field['pseudoconstant'])) {
    $type = 'pseudoconstant';
  }
  else {
    $type = $field['type'];
  }

  switch($type) {
    case CRM_Utils_Type::T_INT:
    case CRM_Utils_Type::T_FLOAT:
      return array(
        'id' => 'numeric',
      );
    case CRM_Utils_Type::T_ENUM:
    case CRM_Utils_Type::T_STRING:
      return array(
        'id' => 'standard',
      );
    case CRM_Utils_Type::T_TEXT:
    case CRM_Utils_Type::T_LONGTEXT:
      // @Todo: Override markup to allow configurable text formats
      return array(
        'id' => 'markup',
      );
    case CRM_Utils_Type::T_BOOLEAN:
      return array(
        'id' => 'boolean',
      );
    case CRM_Utils_Type::T_URL:
      return array(
        'id' => 'url',
      );
    case CRM_Utils_Type::T_DATE:
    case CRM_Utils_Type::T_TIMESTAMP:
      return array(
        'id' => 'civicrm_date',
      );
    case 'pseudoconstant':
      return array(
        'id' => 'civicrm_pseudoconstant',
        'pseudo callback' => "{$entity['dao']}::buildOptions",
        'pseudo arguments' => array($field['name']),
      );
    default:
      return array(
        'id' => 'standard',
      );
  }
}

/**
 * T_INT        = 1,
 * T_STRING     = 2,
 * T_ENUM       = 2,
 * T_DATE       = 4,
 * T_TIME       = 8,
 * T_BOOLEAN    = 16,
 * T_TEXT       = 32,
 * T_LONGTEXT   = 32,
 * T_BLOB       = 64,
 * T_TIMESTAMP  = 256,
 * T_FLOAT      = 512,
 * T_MONEY      = 1024,
 * T_EMAIL      = 2048,
 * T_URL        = 4096,
 * T_CCNUM      = 8192,
 * T_MEDIUMBLOB = 16384;
 *
 * @Todo: sorting for dates
 */
function _civicrm_views_type_to_sort_plugin($type) {
  Drupal::service('civicrm');

  switch($type) {
    default:
      return array(
        'id' => 'standard',
      );
  }
}

/**
 * T_INT        = 1,
 * T_STRING     = 2,
 * T_ENUM       = 2,
 * T_DATE       = 4,
 * T_TIME       = 8,
 * T_BOOLEAN    = 16,
 * T_TEXT       = 32,
 * T_LONGTEXT   = 32,
 * T_BLOB       = 64,
 * T_TIMESTAMP  = 256,
 * T_FLOAT      = 512,
 * T_MONEY      = 1024,
 * T_EMAIL      = 2048,
 * T_URL        = 4096,
 * T_CCNUM      = 8192,
 * T_MEDIUMBLOB = 16384;
 *
 * @Todo: filtering for dates
 */
function _civicrm_views_type_to_filter_plugin($entity, $field) {
  Drupal::service('civicrm');

  if (!empty($field['pseudoconstant'])) {
    $type = 'pseudoconstant';
  }
  else {
    $type = $field['type'];
  }

  switch ($type) {
    case 'pseudoconstant':
      return array(
        'id' => 'civicrm_in_operator',
        'options callback' => "{$entity['dao']}::buildOptions",
        'options arguments' => array($field['name']),
      );
    default:
      return array(
        'id' => 'standard',
      );
  }
}

function _civicrm_views_field_to_relationship_plugin($entity, $field) {
  if (empty($field['FKClassName'])) {
    return array();
  }

  switch ($field['FKClassName']) {
    case 'CRM_Contact_DAO_Contact':
      return array(
        'base' => 'civicrm_contact',
        'id' => 'standard',
        'title' => 'CiviCRM Contact',
        'label' => "CiviCRM Contact (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Contact (from {$entity['display_name']}.{$field['name']})",
      );
    case 'CRM_Event_DAO_Event':
      return array(
        'base' => 'civicrm_event',
        'id' => 'standard',
        'title' => 'CiviCRM Event',
        'label' => "CiviCRM Event (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Event (from {$entity['display_name']}.{$field['name']})",
      );
    case 'CRM_Member_DAO_MembershipType':
      return array(
        'base' => 'civicrm_membership_type',
        'base field' => 'id',
        'id' => 'standard',
        'title' => 'CiviCRM Membership Type',
        'label' => "CiviCRM Membership Type (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Membership Type (from {$entity['display_name']}.{$field['name']})",
      );
    case 'CRM_Contact_DAO_RelationshipType':
      return array(
        'base' => 'civicrm_relationship_type',
        'base field' => 'id',
        'id' => 'standard',
        'title' => 'CiviCRM Relationship Type',
        'label' => "CiviCRM Relationship Type (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Relationship Type (from {$entity['display_name']}.{$field['name']})",
      );
    case 'CRM_Contact_DAO_Group':
      return array(
        'base' => 'civicrm_group',
        'id' => 'standard',
        'title' => 'CiviCRM Group',
        'label' => "CiviCRM Group (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Group (from {$entity['display_name']}.{$field['name']})",
      );
    default:
      return array();
  }
}