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
 * Unchanged anonymous code contribution. No claim.
 *
 * civicrm_handler_field_contact_link.inc
 * Display text data and has the option to link to it's related contact.
 *
 */
class civicrm_handler_field_contact_link extends views_handler_field {

  /**
   * Constructor to provide additional field to add.
   */
  public function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_civicrm_contact'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to node option
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm_contact'] = array(
      '#title' => t('Link this field to its CiviCRM Contact'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_civicrm_contact']),
    );
  }

  /**
   * Render whatever the data is as a link to the node.
   *
   * Data should be made XSS safe prior to calling this function.
   */
  public function render_link($data, $values) {
    if (!empty($this->options['link_to_civicrm_contact']) && user_access('access CiviCRM') && $data !== NULL && $data !== '') {
      return civicrm_views_href($data,
        'civicrm/contact/view',
        "reset=1&cid={$values->{$this->aliases['id']}}"
      );
    }
    else {
      return $data;
    }
  }

  public function render($values) {
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }

}
