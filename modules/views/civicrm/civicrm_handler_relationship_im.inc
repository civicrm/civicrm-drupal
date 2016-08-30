<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.7                                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/

/**
 * Field handler to provide relationship to CiviCRM Instant Messaging.
 *
 * @ingroup civicrm_relationship_handlers
 */
class civicrm_handler_relationship_im extends civicrm_handler_relationship_location {

  static $_provider;
  function construct() {
    parent::construct();
    if (!self::$_provider) {
      if (!civicrm_initialize()) {
        return;
      }
      self::$_provider = CRM_Core_PseudoConstant::get('CRM_Core_DAO_IM', 'provider_id');
    }
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['provider'] = array('default' => 0);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $imOptions = array(0 => 'Any');
    foreach (self::$_provider as $id => $type) {
      $imOptions[$id] = $type;
    }
    $form['provider'] = array(
      '#type' => 'radios',
      '#title' => 'Instant Messaging provider for this relationship',
      '#options' => $imOptions,
      '#description' => t('Instant Messaging provider to be used for this relationship'),
      '#default_value' => $this->options['provider'],
      '#fieldset' => 'location_choices',
    );
  }

  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      if (!method_exists($this->query, 'ensure_table')) {
        vpr_trace();
        exit;
      }
      if (isset($this->options['provider']) && $this->options['provider']) {
        $join    = $this->get_join();
        $extra   = parent::location_extras();
        $extra[] = array(
          'value' => $this->options['provider'],
          'numeric' => TRUE,
          'field' => 'provider_id',
          'operator' => '=',
        );
        $join->extra       = $extra;
        $join->extra_type  = self::$_locationOps[$this->options['location_op']];
        $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
      }
      else {
        $join              = $this->get_join();
        $join->extra       = parent::location_extras();
        $join->extra_type  = self::$_locationOps[$this->options['location_op']];
        $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
      }
    }
    return $this->table_alias;
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Just ensure the one table. This is briefer than parent::query(), which
    // attempts to create an additional join.
    $this->ensure_my_table();
  }
}

