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
 * Field handler to provide a formatted monetary string
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_money extends views_handler_field {

  public function construct() {
    parent::construct();
    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Utils/Money.php';

    if (!empty($this->definition['currency field'])) {
      $this->additional_fields['currency'] = array('field' => $this->definition['currency field']);
    }
  }

  public function render($values) {
    $value = $this->get_value($values);
    if (!empty($this->definition['currency field'])) {
      $currency = $this->get_value($values, 'currency');
    }
    else {
      $currency = CRM_Core_Config::singleton()->defaultCurrency;
    }

    switch ($this->options['display_format']) {
      case 'formatted':
        return CRM_Utils_Money::format($value, $currency);

      case 'raw':
        return $value;
    }
  }

  public function option_definition() {
    $options = parent::option_definition();

    $options['display_format'] = array('default' => 'formatted');

    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['display_format'] = array(
      '#type' => 'select',
      '#title' => t('Display format'),
      '#options' => array(
        'formatted' => t('Currency formatted amount ($123.45)'),
        'raw' => t('Raw amount (123.45)'),
      ),
      '#default_value' => $this->options['display_format'],
    );
  }

}
