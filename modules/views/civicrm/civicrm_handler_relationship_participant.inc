<?php

/**
 * This relationship handler is used when joining the civicrm_relationship table
 * to the civicrm_contact table. This handler allows us to optionally add conditions
 * to the join clause based on relationship_type_id and is_active.
 */
class civicrm_handler_relationship_participant extends views_handler_relationship {
  static $participant_types;

  /**
   * Preload the list of relationship_types and store in the static variable
   * $relationship_types
   */
  public function construct() {
    parent::construct();

    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Core/PseudoConstant.php';

    self::$participant_types = CRM_Core_PseudoConstant::participantRoles();
  }

  /**
   * Add additional options for relationship_type and relationship_state
   * to the view. By defining these here, Views will take care of saving the
   * values submitted from the options form.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['participant_type'] = array('default' => 0);
    return $options;
  }

  /**
   * Add relationship_type drowndown and relationship_state checkbox to
   * relationship configuration form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['participant_type'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => 'Choose a specific Participant type',
      '#options' => self::$participant_types,
      '#description' => t('Choose to limit this relationship to one or more specific types of CiviCRM Participant.'),
      '#default_value' => $this->options['participant_type'],
    );
  }

  /**
   * Modify the default views relationship query to optionally specify
   * join conditions for relationship_type or is_active (relationship_state).
   */
  public function query() {
    parent::query();

    // Specify the type of relationships to join
    if (isset($this->options['participant_type']) && $this->options['participant_type']) {
      $this->query->table_queue[$this->alias]['join']->extra[] = array(
        'value' => $this->options['participant_type'],
        'numeric' => TRUE,
        'field' => 'role_id',
      );
    }
  }

}
