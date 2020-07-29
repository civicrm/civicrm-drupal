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
 * Field handler for Instant Messaging field
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_im extends civicrm_handler_field_location {
  public static $_provider;

  public function construct() {
    parent::construct();
    if (!self::$_provider) {
      if (!civicrm_initialize()) {
        return;
      }
      self::$_provider = CRM_Core_PseudoConstant::get('CRM_Core_DAO_IM', 'provider_id');
    }
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['provider'] = array('default' => 0);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $imOptions = array(0 => 'Any');
    foreach (self::$_provider as $id => $type) {
      $imOptions[$id] = $type;
    }
    $form['provider'] = array(
      '#type' => 'radios',
      '#title' => 'Instant Messaging provider for this field',
      '#options' => $imOptions,
      '#description' => t('Instant Messaging provider to be displayed for this field'),
      '#default_value' => $this->options['provider'],
      '#fieldset' => 'location_choices',
    );
  }

  public function join_im($join = array()) {
    $extra = array();
    if (isset($join->extra)) {
      $extra = $join->extra;
    }
    if (isset($this->options['provider']) && $this->options['provider']) {
      $extra[] = array(
        'value' => $this->options['provider'],
        'numeric' => TRUE,
        'field' => 'provider_id',
        'operator' => '=',
      );
    }
    if (!empty($extra)) {
      $join->extra = $extra;
    }
    return $join;
  }

  public function get_join() {
    $join = parent::get_join();
    $join = $this->join_im($join);
    return $join;
  }

  public function ensure_my_table() {
    if (!isset($this->table_alias)) {
      if (!method_exists($this->query, 'ensure_table')) {
        vpr_trace();
        exit;
      }
      $join = $this->get_join();
      if (empty($this->relationship)) {
        //https://www.drupal.org/node/2617032
        $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
      }
      else {
        $this->table_alias = $this->query->ensure_table($this->table, $this->relationship, $join);
      }
    }
    return $this->table_alias;
  }

}
