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
 * Generic field handler for address and location fields
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_address extends civicrm_handler_field_location {

  public function option_definition() {
    $options = parent::option_definition();
    $options['is_billing'] = array('default' => 0);
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['is_billing'] = array(
      '#type' => 'checkbox',
      '#title' => 'Show only Billing Address record?',
      '#options' => array(0 => 'No', 1 => 'Yes'),
      '#description' => t('Check above box if you want only the <strong>Billing Address</strong> record displayed.'),
      '#default_value' => $this->options['is_billing'],
      '#fieldset' => 'location_choices',
    );
  }

  public function join_address($join = array()) {
    $extra = array();
    if (isset($join->extra)) {
      $extra = $join->extra;
    }
    if (isset($this->options['is_billing']) && $this->options['is_billing']) {
      $extra[] = array(
        'value' => $this->options['is_billing'],
        'numeric' => TRUE,
        'field' => 'is_billing',
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
    $join = $this->join_address($join);
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
