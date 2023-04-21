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
 * @file Contains a plugin to extract CiviCRM IDs from the URL.
 */

/**
 * Class civicrm_plugin_argument_default_civicrm_id
 */
class civicrm_plugin_argument_default_civicrm_id extends views_plugin_argument_default {

  /**
   * List the options relevant for this plugin.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['id_type'] = array('default' => 'Contact');
    return $options;
  }

  /**
   * Provide the default form for setting options.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['id_type'] = array(
      '#type' => 'select',
      '#title' => t('CiviCRM ID type'),
      '#description' => t('Select the entity type of the ID to extract from the URL'),
      '#options' => array(
        'Contact' => t('Contact'),
        'Contribution page' => t('Contribution page'),
        'Event Info page' => t('Event Info page'),
      ),
      '#default_value' => $this->options['id_type'],
      '#required' => TRUE,
    );
  }

  /**
   * Get the default argument.
   */
  public function get_argument() {

    switch ($this->options['id_type']) {

      // Contact ID is set using the param 'cid'
      case 'Contact':
        if (!empty($_GET['cid']) && is_numeric($_GET['cid'])) {
          return $_GET['cid'];
        }
        break;

      // Contribution page ID is set using the param 'id'
      case 'Contribution page':
        if (
          strpos(current_path(), 'contribute/transact') !== FALSE
          && !empty($_GET['id'])
          && is_numeric($_GET['id'])
        ) {
          return $_GET['id'];
        }
        break;

      case 'Event Info page':
        if (
          strpos(current_path(), 'event/info') !== FALSE
          && !empty($_GET['id'])
          && is_numeric($_GET['id'])
        ) {
          return $_GET['id'];
        }
        break;
    }

    // Return FALSE if we haven't returned yet
    return FALSE;

  }

}
