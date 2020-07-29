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
 * Field handler to render a custom field with options
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_custom extends views_handler_field {

  public function construct() {
    parent::construct();
    civicrm_initialize();
  }

  public function option_definition() {
    $options = parent::option_definition();
    $options['civicrm_custom_formatter'] = array(
      'default' => 'label',
    );
    return $options;
  }

  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // Replicate Drupal UI for formatter options, but with CiviCRM
    // terminology.
    $form['civicrm_custom_formatter'] = array(
      '#title' => 'Formatter',
      '#type' => 'select',
      '#default_value' => $this->options['civicrm_custom_formatter'],
      '#options' => array(
        'label' => t('Label'),
        'value' => t('Value'),
      ),
    );
  }

  public function render($values) {
    $value = $values->{$this->field_alias};
    switch ($this->options['civicrm_custom_formatter']) {
      case 'value':
        $value = trim($value, CRM_Core_DAO::VALUE_SEPARATOR);
        $value = implode(', ', explode(CRM_Core_DAO::VALUE_SEPARATOR, $value));
        return $value;

      case 'label':
      default:
        if (!is_null($value)) {
          // get the field id from the db
          if (!empty($this->definition['title'])) {
            $customFieldID = CRM_Core_DAO::getFieldValue('CRM_Core_BAO_CustomField', $this->real_field, 'id', 'column_name');
            return CRM_Core_BAO_CustomField::displayValue($value, $customFieldID);
          }
          // could not get custom id, lets just return what we have
          return $value;
        }
        return NULL;
    }
  }

}
