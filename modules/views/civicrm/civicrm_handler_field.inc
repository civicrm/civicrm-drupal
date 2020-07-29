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
 * Field handler to provide simple renderer that allows linking to a user.
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field extends views_handler_field {

  /**
   * Override init function to provide generic option to link to user.
   */
  public function init(&$view, &$data) {
    parent::init($view, $data);
  }

  /**
   * Provide link to node option
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_civicrm'] = array(
      '#title' => t('Link this field to its user'),
      '#type' => 'checkbox',
      '#default_value' => isset($this->options['link_to_civicrm']) ? $this->options['link_to_civicrm'] : '',
    );
  }

  public function render_link($data, $values) {
    $data = str_replace('', ', ', trim($data, ''));
    if (!empty($this->options['link_to_civicrm']) && user_access('access CiviCRM') && $values->id) {
      return civicrm_views_href($data,
        'civicrm/contact/view',
        "reset=1&cid={$values->id}"
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
