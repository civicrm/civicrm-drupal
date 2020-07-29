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

/*
 * Copyright (C) 2009 James Rakich
 * Licensed to CiviCRM under the Academic Free License version 3.0.
 */

/**
 * civicrm_handler_field_link_event.inc
 * Completely customizable text that can be linked to numerous places related to the event.
 * Info, Register, Config etc pages, as well as a custom Drupal Link with the Event ID appended,
 * which is great for linking to other views using the event or participant base table.
 */

/**
 * Field handler to provide simple renderer that allows linking to various pages involving the event.
 */
class civicrm_handler_field_link_event extends views_handler_field {

  /**
   * Constructor to provide additional field to add.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_civicrm_event'] = array('default' => 'view');
    $options['custom_link'] = array('default' => '');
    $options['link_text'] = array('default' => '');
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm_event'] = array(
      '#type' => 'select',
      '#title' => t('Choose where to link this field'),
      '#options' => array(
        'page' => t('Link to Event Page'),
        'register' => t('Link to Registration Page'),
        'config' => t('Link to Event Configuration'),
        'listing' => t('Link to Participant Listing'),
        'participants' => t('Link to Edit Event Participants'),
        'delete' => t('Link to Delete Event'),
        'custom' => t('Link to a Custom Node'),
      ),
      '#default_value' => $this->options['link_to_civicrm_event'],
      '#required' => TRUE,
    );

    $form['custom_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Link to a Drupal Node'),
      '#description' => t('Link to a Drupal Node or View, from the base path. The Event ID will be appended to the end of the link.'),
      '#default_value' => isset($this->options['custom_link']) ? $this->options['custom_link'] : '',
      '#dependency' => array('edit-options-link-to-civicrm-event' => array('custom')),
    );

    $form['link_text'] = array(
      '#type' => 'textfield',
      '#title' => t('Link Text'),
      '#description' => t('The text that will display in the field with the link.'),
      '#default_value' => isset($this->options['link_text']) ? $this->options['link_text'] : '',
      '#required' => TRUE,
    );
  }

  public function render_link($data, $values) {

    $link_text = $this->options['link_text'];
    $link_choice = $this->options['link_to_civicrm_event'];

    if ($link_choice == 'custom') {
      $custom_link = $this->options['custom_link'];
    }

    switch ($link_choice) {

      // LINKING TO EVENT INFO

      case 'page':
        if (user_access('view event info') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/event/info',
            "id={$values->id}&reset=1"
          );
        }

        // LINKING TO EVENT REGISTRATION
      case 'register':
        if (user_access('register for events') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/event/register',
            "id={$values->id}&reset=1"
          );
        }

        // LINKING TO EVENT CONFIG
      case 'config':
        if (user_access('access CiviEvent') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/event/manage',
            "action=update&id={$values->id}&reset=1"
          );
        }

        // LINKING TO EVENT PARTICIPANT LISTING
      case 'listing':
        if (user_access('view event participants') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/event/participant',
            "reset=1&id={$values->id}"
          );
        }

        // LINKING TO EVENT PARTICIPANTS
      case 'participants':
        if (user_access('edit event participants') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/event/search',
            "reset=1&force=1&event={$values->id}"
          );
        }

        // LINKING TO EVENT DELETION
      case 'delete':
        if (user_access('access CiviEvent') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/event/manage',
            "action=delete&id={$values->id}"
          );
        }

        // CUSTOM NODE LINKAGE, GOOD FOR LINKING TO OTHER VIEWS WITH AN ARGUMENT
      case 'custom':
        if ($custom_link !== NULL && $custom_link !== '' && $link_text !== NULL && $link_text !== '') {
          return l($link_text, $custom_link . "{$values->id}");
        }

        // see CRM-CRM-14251 - if we return null here then views is able to be configured as to what to do
      default:
        return NULL;
    }
  }

  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function render($values) {
    return $this->render_link(NULL, $values);
  }

}
