<?php

function civicrm_views_views_data() {
  $data = array();

  try {
    \Drupal::service('civicrm');
  }
  catch (Exception $e) {
    \Drupal::logger('civicrm_views')->error("Unable to initialize CiviCRM. Aborting hook_views_data().");
    return $data;
  }

  // Use CiviCRM schema data to automatically populate $data array with fields and
  // make intelligent guesses for field, sort and filter plugins.
  foreach (_civicrm_views_civicrm_entities() as $table => $civicrm_entity) {
    $data[$table]['table']['group'] = $civicrm_entity['display_name'];

    // Only define the base key if we intend for the current CiviCRM table to
    // be a base table in views (and not just a table that is brought in via a relationship
    // or an implicit join) ie. a table that you can start building a view from.
    if ($civicrm_entity['base']) {
      $data[$table]['table']['base'] = array(
        'field' => 'id',
        'title' => $civicrm_entity['display_name'],
        //'database' => 'civicrm',
      );
    };

    $fields = $civicrm_entity['dao']::fields();
    //ksm($fields);
    foreach ($fields as $field) {
      $data[$table][$field['name']] = array(
        'title' => !empty($field['title']) ? $field['title'] : "{$civicrm_entity['display_name']}.{$field['name']}",
        'help' => !empty($field['title']) ? $field['title'] : "{$civicrm_entity['display_name']}.{$field['name']}",
        'field' => _civicrm_views_field_to_field_plugin($civicrm_entity, $field),
        'sort' => _civicrm_views_field_to_sort_plugin($field['type']),
        'filter' => _civicrm_views_field_to_filter_plugin($civicrm_entity, $field),
        'relationship' => array(),
      );
    }
  }

  _civicrm_views_add_relationships($data);
  _civicrm_views_add_implicit_joins($data);

  ksm($data);
  return $data;
}

/**
 * @Todo: Contributions
 */
function _civicrm_views_civicrm_entities() {
  return array(
    'civicrm_activity' => array(
      'display_name' => t('CiviCRM Activity'),
      'dao' => 'CRM_Activity_DAO_Activity',
      'base' => TRUE,
    ),
    'civicrm_address' => array(
      'display_name' => t('CiviCRM Address'),
      'dao' => 'CRM_Core_DAO_Address',
      'base' => TRUE,
    ),
    'civicrm_contact' => array(
      'display_name' => t('CiviCRM Contact'),
      'dao' => 'CRM_Contact_DAO_Contact',
      'base' => TRUE,
    ),
    'civicrm_email' => array(
      'display_name' => t('CiviCRM Email'),
      'dao' => 'CRM_Core_DAO_Email',
      'base' => FALSE,
    ),
    'civicrm_entity_tag' => array(
      'display_name' => t('CiviCRM Tag'),
      'dao' => 'CRM_Core_DAO_EntityTag',
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
    'civicrm_phone' => array(
      'display_name' => 'CiviCRM Phone',
      'dao' => 'CRM_Core_DAO_Phone',
      'base' => TRUE,
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
}

/**
 * In general, we define a views relationship for one-to-many relationships. A
 * relationship may also be used in the case where an implicit join would be rarely used or would
 * bring in confusing or duplicate fields.
 *
 * This is ordered alphabetically by array key.
 */
function _civicrm_views_add_relationships(&$data) {
  foreach (\CRM_Activity_DAO_ActivityContact::buildOptions('record_type_id') as $record_type_id => $name) {
    $data['civicrm_activity']["id_contact_record_type_{$record_type_id}"] = array(
      'relationship' => array(
        'id' => 'standard',
        'field' => 'id',
        'base' => 'civicrm_activity_contact',
        'base field' => 'activity_id',
        'extra' => array(
          array(
            'field' => 'record_type_id',
            'value' => $record_type_id,
          ),
        ),
        'title' => t('CiviCRM !record_type_name', array('!record_type_name' => $name)),
        'label' => t('CiviCRM !record_type_name (from Activity)', array('!record_type_name' => $name)),
        'help' => t('CiviCRM !record_type_name (from Activity)', array('!record_type_name' => $name)),
      ),
    );
  }
  $data['civicrm_contact']['id_address'] = array(
    'relationship' => array(
      'id' => 'civicrm_location',
      'field' => 'id',
      'base' => 'civicrm_address',
      'base field' => 'contact_id',
      'title' => t('CiviCRM Addresses'),
      'label' => t('CiviCRM Addresses (from Contact.id)'),
      'help' => t('CiviCRM Addresses (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_entity_tag'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_entity_tag',
      'base field' => 'entity_id',
      'extra' => array(
        array(
          'field' => 'entity_table',
          'value' => 'civicrm_contact',
        ),
      ),
      'title' => t('CiviCRM Tags'),
      'label' => t('CiviCRM Tags (from Contact.id)'),
      'help' => t('CiviCRM Tags (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_email'] = array(
    'relationship' => array(
      'id' => 'civicrm_location',
      'field' => 'id',
      'base' => 'civicrm_email',
      'base field' => 'contact_id',
      'title' => t('CiviCRM Emails'),
      'label' => t('CiviCRM Emails (from Contact.id)'),
      'help' => t('CiviCRM Emails (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_group_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_group_contact',
      'base field' => 'contact_id',
      'title' => t('CiviCRM Groups'),
      'label' => t('CiviCRM Groups (from Contact.id)'),
      'help' => t('CiviCRM Groups (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_membership'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_membership',
      'base field' => 'contact_id',
      'title' => t('CiviCRM Memberships'),
      'label' => t('CiviCRM Memberships (from Contact.id)'),
      'help' => t('CiviCRM Memberships (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_participant'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_participant',
      'base field' => 'contact_id',
      'title' => t('CiviCRM Participants'),
      'label' => t('CiviCRM Participants (from Contact.id)'),
      'help' => t('CiviCRM Participants (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_phone'] = array(
    'relationship' => array(
      'id' => 'civicrm_location',
      'field' => 'id',
      'base' => 'civicrm_phone',
      'base field' => 'contact_id',
      'title' => t('CiviCRM Phones'),
      'label' => t('CiviCRM Phones (from Contact.id)'),
      'help' => t('CiviCRM Phones (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_relationship_a'] = array(
    'relationship' => array(
      'id' => 'civicrm_relationship',
      'field' => 'id',
      'base' => 'civicrm_relationship',
      'base field' => 'contact_id_a',
      'title' => t('CiviCRM Relationship as Contact A'),
      'label' => t('CiviCRM Relationship as Contact A (from Contact.id)'),
      'help' => t('CiviCRM Relationship as Contact A (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['id_relationship_b'] = array(
    'relationship' => array(
      'id' => 'civicrm_relationship',
      'field' => 'id',
      'base' => 'civicrm_relationship',
      'base field' => 'contact_id_b',
      'title' => t('CiviCRM Relationship as Contact B'),
      'label' => t('CiviCRM Relationships as Contact B (from Contact.id)'),
      'help' => t('CiviCRM Relationships as Contact B (from Contact.id)'),
    ),
  );
  $data['civicrm_contact']['employer_id_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'employer_id',
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'title' => t('CiviCRM Employer'),
      'label' => t('CiviCRM Empolyer (from Contact.employer_id'),
      'help' => t('CiviCRM Employer (from Contact.employer_id'),
    ),
  );
  $data['civicrm_contact']['primary_contact_id_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'primary_contact_id',
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'title' => t('CiviCRM Primary Contact'),
      'label' => t('CiviCRM Primary Contact (from Contact.primary_contact_id'),
      'help' => t('CiviCRM Primary Contact (from Contact.primary_contact_id'),
    ),
  );
  $data['civicrm_entity_tag']['entity_id_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'entity_id',
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'extra' => array(
        array(
          'table' => 'civicrm_entity_tag',
          'field' => 'entity_table',
          'value' => 'civicrm_contact',
        ),
      ),
      'title' => t('CiviCRM Contacts'),
      'label' => t('CiviCRM Contacts (from Tag.entity_id)'),
      'help' => t('CiviCRM Contacts (from Tag.entity_id)'),
    ),
  );
  $data['civicrm_event']['id_participant'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_participant',
      'base field' => 'event_id',
      'title' => t('CiviCRM Participants'),
      'label' => t('CiviCRM Participants (from Event.id)'),
      'help' => t('CiviCRM Participants (from Event.id)'),
    ),
  );
  $data['civicrm_group']['id_group_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'id',
      'base' => 'civicrm_group_contact',
      'base field' => 'group_id',
      'title' => t('CiviCRM Group Contacts'),
      'label' => t('CiviCRM Group Contacts (from Group.id)'),
      'help' => t('CiviCRM Group Contacts (from Group.id)'),
    ),
  );
  $data['civicrm_relationship']['contact_id_a_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'contact_id_a',
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'title' => t('CiviCRM Contact A'),
      'label' => t('CiviCRM Contact A (from Relationship.contact_id_a)'),
      'help' => t('CiviCRM Contact A (from Relationship.contact_id_a)'),
    ),
  );
  $data['civicrm_relationship']['contact_id_b_contact'] = array(
    'relationship' => array(
      'id' => 'standard',
      'field' => 'contact_id_b',
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'title' => t('CiviCRM Contact B'),
      'label' => t('CiviCRM Contact B (from Relationship.contact_id_b)'),
      'help' => t('CiviCRM Contact B (from Relationship.contact_id_b)'),
    ),
  );
  $data['civicrm_relationship_type']['id_relationship'] = array(
    'id' => 'standard',
    'field' => 'id',
    'base' => 'civicrm_relationship',
    'base field' => 'relationship_type_id',
    'title' => t('CiviCRM Relationships'),
    'label' => t('CiviCRM Relationships (from Relationship.id)'),
    'help' => t('CiviCRM Relationships (from Relationship.id)'),
  );
}

/**
 * We only define an implicit join in the case of a one-to-one relationship
 * and where the additional fields that the join makes visible are intuitively understood.
 *
 * This is ordered alphabetically by array key.
 */
function _civicrm_views_add_implicit_joins(&$data) {
  $data['civicrm_contact']['table']['join'] = array(
    'civicrm_activity_contact' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    'civicrm_group_contact' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    'civicrm_membership' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    'civicrm_participant' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
  );
  $data['civicrm_group']['table']['join'] = array(
    'civicrm_group_contact' => array(
      'left_field' => 'group_id',
      'field' => 'id',
    ),
  );
  $data['civicrm_event']['table']['join'] = array(
    'civicrm_participant' => array(
      'left_field' => 'event_id',
      'field' => 'id',
    ),
  );
  $data['civicrm_membership_type']['table']['join'] = array(
    'civicrm_membership' => array(
      'left_field' => 'membership_type_id',
      'field' => 'id',
    ),
  );
  $data['civicrm_relationship_type']['table']['join'] = array(
    'civicrm_relationship' => array(
      'left_field' => 'relationship_type_id',
      'field' => 'id',
    ),
  );
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
  $type = !empty($field['pseudoconstant']) ? 'pseudoconstant' : $field['type'];

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
function _civicrm_views_field_to_sort_plugin($type) {
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
function _civicrm_views_field_to_filter_plugin($entity, $field) {
  $type = !empty($field['pseudoconstant']) ? 'pseudoconstant' : $field['type'];

  switch ($type) {
    case CRM_Utils_Type::T_BOOLEAN:
      return array(
        'id' => 'boolean',
      );
    case CRM_Utils_Type::T_INT:
    case CRM_Utils_Type::T_FLOAT:
    case CRM_Utils_Type::T_MONEY:
      return array(
        'id' => 'numeric',
      );
    case CRM_Utils_Type::T_ENUM:
    case CRM_Utils_Type::T_STRING:
    case CRM_Utils_Type::T_TEXT:
    case CRM_Utils_Type::T_LONGTEXT:
    case CRM_Utils_Type::T_URL:
    case CRM_Utils_Type::T_EMAIL:
      return array(
        'id' => 'string',
      );
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
      // @Todo: delete this?
      return array(
        'base' => 'civicrm_event',
        'id' => 'standard',
        'title' => 'CiviCRM Event',
        'label' => "CiviCRM Event (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Event (from {$entity['display_name']}.{$field['name']})",
      );
    case 'CRM_Member_DAO_MembershipType':
      // @Todo: remove this?
      return array(
        'base' => 'civicrm_membership_type',
        'base field' => 'id',
        'id' => 'standard',
        'title' => 'CiviCRM Membership Type',
        'label' => "CiviCRM Membership Type (from {$entity['display_name']}.{$field['name']})",
        'help' => "CiviCRM Membership Type (from {$entity['display_name']}.{$field['name']})",
      );
    case 'CRM_Contact_DAO_RelationshipType':
      // @Todo: remove this?
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