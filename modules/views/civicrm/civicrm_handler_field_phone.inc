<?php
// $Id$

class civicrm_handler_field_phone extends views_handler_field {
  static $_phoneType;
  function construct() {
    if (!self::$_phoneType) {
      if (!civicrm_initialize()) {
        return;
      }
      require_once 'CRM/Core/PseudoConstant.php';
      self::$_phoneType = CRM_Core_PseudoConstant::phoneType();
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['phone_type'] = array('default' => '');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $phoneOptions = array(0 => 'Any');
    foreach (self::$_phoneType as $id => $type) {
      $phoneOptions[$id] = $type;
    }
    $form['phone_type'] = array(
      '#type' => 'radios',
      '#title' => 'Phone type for this field',
      '#options' => $phoneOptions,
      '#description' => t('Phone type to be used for this field'),
      '#default_value' => $this->options['phone_type'],
    );
  }

  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      if (!method_exists($this->query, 'ensure_table')) {
        vpr_trace();
        exit;
      }
      if (isset($this->options['phone_type']) && $this->options['phone_type']) {
        $join = $this->get_join();
        $join->extra = array(
          array(
            'value' => $this->options['phone_type'],
            'numeric' => TRUE,
            'field' => 'phone_type_id',
          ),
        );
      }
      //$this->table_alias = $this->query->ensure_table($this->table, $this->relationship, $join);
      $this->table_alias = $this->query->add_table($this->table, $this->telationship, $join);
    }
    return $this->table_alias;
  }
}

