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
 * Field handler to provide acess control for the state field (which is a lookup)
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_state extends civicrm_handler_field_address {
  public static $_states;
  public static $_states_full;

  public function construct() {
    parent::construct();
    if (!self::$_states || !self::$_states_full) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/PseudoConstant.php';
      self::$_states = CRM_Core_PseudoConstant::stateProvinceAbbreviation();
      self::$_states_full = CRM_Core_PseudoConstant::stateProvince();
    }
  }

  public function render($values) {
    $sid = $values->{$this->field_alias};
    if (empty($sid) || (int) $sid <= 0) {
      return NULL;
    }

    if ($this->options['prov_display'] == 1) {
      return isset(self::$_states[$values->{$this->field_alias}])
        ? self::$_states[$values->{$this->field_alias}] : NULL;
    }
    else {
      return isset(self::$_states_full[$values->{$this->field_alias}])
        ? self::$_states_full[$values->{$this->field_alias}] : NULL;
    }
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['prov_display'] = array('default' => '');
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['prov_display'] = array(
      '#type' => 'radios',
      '#title' => 'Display complete name or abbreviation',
      '#options' => array(1 => 'Display state/province abbreviations', 2 => 'Display full state/province name'),
      '#description' => t('Display full or abbreviated province name'),
      '#default_value' => $this->options['prov_display'],
    );
  }

}
