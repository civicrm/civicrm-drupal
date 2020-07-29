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
 * Field handler to present a link to the user.
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_link extends civicrm_handler_field {

  public function construct() {
    parent::construct();
    $this->additional_fields['id'] = 'id';
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['text'] = array('default' => '', 'translatable' => TRUE);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
  }

  /**
   * An example of field level access control.
   */
  public function access() {
    return user_access('access CiviCRM');
  }

  public function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('view');
    $id = $values->{$this->aliases['id']};
    return civicrm_views_href($text, "civicrm/contact/view", "reset=1&id=$id");
  }

}
