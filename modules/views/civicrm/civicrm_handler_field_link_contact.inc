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
 * Copyright (C) 2009 James Rakich
 * Licensed to CiviCRM under the Academic Free License version 3.0.
 */

/**
 * civicrm_handler_field_link_contact
 *
 * Completely customizable text that can be linked to numerous places related to the activity.
 * View, Edit and Delete Pages, as well as a custom Drupal Link with the Contact ID appended,
 * which is great for linking to other views using contact or other base tables.
 *
 */

/**
 * Field handler to provide simple renderer that allows linking to various pages involving the contact.
 */
class civicrm_handler_field_link_contact extends views_handler_field {

  /**
   * Constructor to provide additional field to add.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_civicrm_contact'] = array('default' => 'view');
    $options['custom_link'] = array('default' => '');
    $options['link_text'] = array('default' => '');
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm_contact'] = array(
      '#type' => 'select',
      '#title' => t('Choose where to link this field'),
      '#options' => array(
        'view' => t('Link to View Contact'),
        'edit' => t('Link to Edit Contact'),
        'delete' => t('Link to Delete Contact'),
        'custom' => t('Link to a Custom Node'),
      ),
      '#default_value' => $this->options['link_to_civicrm_contact'],
      '#required' => TRUE,
    );

    $form['custom_link'] = array(
      '#type' => 'textfield',
      '#title' => t('Custom Link to a Drupal Node'),
      '#description' => t('Link to a Drupal Node or View, from the base path. The Contact ID will be appended to the end of the link.'),
      '#default_value' => isset($this->options['custom_link']) ? $this->options['custom_link'] : '',
      '#dependency' => array('edit-options-link-to-civicrm-contact' => array('custom')),
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
    $link_choice = $this->options['link_to_civicrm_contact'];
    if ($link_choice == 'custom') {
      $custom_link = $this->options['custom_link'];
    }

    switch ($link_choice) {

      // LINKING TO CONTACT VIEW PAGE

      case 'view':
        if (user_access('view all contacts') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/contact/view',
            "reset=1&cid={$values->id}"
          );
        }
        break;

      // LINKING TO CONTACT EDIT PAGE
      case 'edit':
        if (user_access('edit all contacts') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/contact/add',
            "reset=1&action=update&cid={$values->id}"
          );
        }
        break;

      // LINKING TO CONTACT DELETE PAGE
      case 'delete':
        if (user_access('edit all contacts') && $link_text !== NULL && $link_text !== '') {
          return civicrm_views_href($link_text,
            'civicrm/contact/view/delete',
            "reset=1&delete=1&cid={$values->id}"
          );
        }
        break;

      // CUSTOM NODE LINKAGE, GOOD FOR LINKING TO OTHER VIEWS WITH AN ARGUMENT
      case 'custom':
        if ($custom_link !== NULL && $custom_link !== '' && $link_text !== NULL && $link_text !== '') {
          return l($link_text, $custom_link . "{$values->id}");
        }
        break;

    }
    return $link_text;
  }

  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function render($values) {
    return $this->render_link(check_plain(property_exists($values, $this->field_alias)), $values);
  }

}
