<?php
// $Id$

//
require_once 'civicrm_rules_entities_utils.inc';
module_load_include('inc', 'civicrm_rules', 'civicrm_rules_civicrm_event.eval');
module_load_include('inc', 'civicrm_rules', 'civicrm_rules_civicrm_participant.eval');
module_load_include('inc', 'civicrm_rules', 'civicrm_rules_civicrm_contact.eval');
/*
 * Declare post functions for valid post entities. This declares 5 actions for each enabled object
 * Enabled objects are defined through the 'civicrm_rules_post_entities' drupal variable 
 */
function civicrm_rules_entities_event_info() {
  $validObjects = variable_get('civicrm_rules_post_entities', array());
  $events = array();

  if (isset($validObjects['contact'])) {
    // Contact is dealt with elsewhere
    unset($validObjects['contact']);
  }
  if (is_array($validObjects)) {

    foreach ($validObjects as $entity => $enabled) {
      if (!$enabled == 0) {
        //todo consider building the entity name into the argument rather than calling the same argument for each
        $events['civicrm_' . $entity . "_create"] = array(
          'label' => t("$entity has been created"),
          'module' => 'CiviCRM ' . $entity,
          'arguments' => civicrm_rules_entities_arguments(t('Created ' . $entity),
            "civicrm_$entity"
          ),
        );
        $events['civicrm_' . $entity . "_edit"] = array(
          'module' => 'CiviCRM ' . $entity,
          'label' => t("$entity has been updated"),
          'arguments' => civicrm_rules_entities_arguments(t('Updated ' . $entity),
            "civicrm_$entity"
          ),
        );
        $events['civicrm_' . $entity . "_view"] = array(
          'module' => 'CiviCRM ' . $entity,
          'label' => t("$entity has been viewed"),
          'arguments' => civicrm_rules_entities_arguments(t('Viewed ' . $entity),
            "civicrm_$entity"
          ),
        );
        $events['civicrm_' . $entity . "_delete"] = array(
          'module' => 'CiviCRM ' . $entity,
          'label' => t("$entity has been deleted"),
          'arguments' => civicrm_rules_entities_arguments(t('Deleted ' . $entity),
            "civicrm_$entity"
          ),
        );
      }
    }
  }
  return $events;
}

/**
 * implementation of hook_rules_condition_info()
 */
function civicrm_rules_entities_condition_info() {

  return array(
    'civicrm_rules_condition_participant_role' => array(
      'label' => t('Participant Role'),
      'arguments' => civicrm_rules_entities_conditions(t('Participant Role'), 'civicrm_participant'),
      'module' => 'CiviCRM participant',
    ),
    'civicrm_rules_condition_participant_status' => array(
      'label' => t('Participant Status'),
      'arguments' => civicrm_rules_entities_conditions(t('Participant'), 'civicrm_participant'),
      'module' => 'CiviCRM participant',
    ),
    'civicrm_rules_condition_event_type' => array(
      'label' => t('Event Type'),
      'arguments' => civicrm_rules_entities_conditions(t('Created Event'), 'civicrm_event'),
      'module' => 'CiviCRM event',
    ),
  );
}

/**
 * Returns conditions available for CiviCRM Rules
 */
function civicrm_rules_entities_conditions($label, $type = 'contact') {

  $default = array($type => array('type' => $type, 'label' => $label));

  return $default;
}

/*
 * Hook Arguments - define the objects that will be loaded by each event & made available for actions
 * @todo restructure this so that either all arguments are in one file by themselves or each entity has it's own file (prefered)
 * @return array arguments - including object Name + label, any specific items & acting user
 */
function civicrm_rules_entities_arguments($label, $type = 'contact') {
  $default = array($type => array('type' => $type,
      'label' => $label,
    )) + rules_events_global_user_argument();
  if ($type == 'civicrm_event') {
    return $default + array(
      'cms_user' => array(
        'type' => 'user',
        'label' => t('User that created the event'),
        'handler' => 'civicrm_rules_events_argument_civicrm_event',
      ),
      'event node' => array(
        'type' => 'node',
        'label' => t('Node related to the event'),
        'handler' => 'civicrm_rules_events_argument_civicrm_eventnode',
      ),
    );
    //TODO add something here to call 'civicrm_rules_events_argument_civicrm_event',
    //with 'contact' as arg & load contact - not required right now but really - this is the point of rules!
  }

  if ($type == 'civicrm_participant') {
    return $default + array(
      $type => array(
        'type' => $type,
        'label' => $label,
      ),
      'event node' => array(
        'type' => 'node',
        'label' => t('Node related to the event'),
        'handler' => 'civicrm_rules_events_argument_civicrm_eventnode',
      ),
      'cms_user' => array(
        'type' => 'user',
        'label' => t('User that registered for the event'),
        'handler' => 'civicrm_rules_events_argument_civicrm_contactID_load_user',
      ),
    );
  }
}

function civicrm_rules_entities_data_type_info() {
  return array(
    'civicrm_event' => array(
      'label' => t('CiviCRM Event'),
      'class' => 'rules_data_type_civicrm_event',
      'savable' => FALSE,
      'identifiable' => TRUE,
    ),
    'civicrm_participant' => array(
      'label' => t('CiviCRM Participant'),
      'class' => 'rules_data_type_civicrm_participant',
      'savable' => FALSE,
      'identifiable' => TRUE,
    ),
  );
}

