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
 * Heavily based on anonymous code contribution. No claim.
 *
 * civicrm_handler_field_country.inc
 * Displays Country after grabbing the label from the database
 *
 */
class civicrm_handler_field_country extends civicrm_handler_field_address {
  public static $_countries;
  public static $_countries_full;

  public function construct() {
    parent::construct();
    if (!self::$_countries) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/PseudoConstant.php';
      self::$_countries = CRM_Core_PseudoConstant::countryIsoCode();
      self::$_countries_full = CRM_Core_PseudoConstant::country();
    }
  }

  public function render($values) {
    $sid = $values->{$this->field_alias};
    if (empty($sid) ||
      (int ) $sid <= 0
    ) {
      return NULL;
    }

    if ($this->options['country_display'] == 1) {
      return self::$_countries[$values->{$this->field_alias}];
    }
    else {
      return self::$_countries_full[$values->{$this->field_alias}];
    }
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['country_display'] = array('default' => '');
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['country_display'] = array(
      '#type' => 'radios',
      '#title' => 'Type of display for country field',
      '#options' => array(1 => 'Display two letter country code', 2 => 'Display full country name'),
      '#description' => t('Display full or abbreviated country name'),
      '#default_value' => $this->options['country_display'],
    );
  }

}
