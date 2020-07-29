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
 * Field handler to provide acess control for the email field
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_mail extends civicrm_handler_field {

  public function option_definition() {
    $options = parent::option_definition();
    $options['link_to_civicrm'] = array('default' => 'mailto');
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm'] = array(
      '#title' => t('Link this field'),
      '#type' => 'radios',
      '#options' => array(
        0 => t('No link'),
        'user' => t('To the user'),
        'mailto' => t("With a mailto:"),
      ),
      '#default_value' => $this->options['link_to_civicrm'],
    );
  }

  public function render($values) {
    if ($this->options['link_to_civicrm'] == 'mailto') {
      return l($values->{$this->field_alias}, "mailto:" . $values->{$this->field_alias});
    }
    return $this->render_link(check_plain($values->{$this->field_alias}), $values);
  }

}
