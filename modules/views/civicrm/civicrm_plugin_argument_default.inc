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
 * Default argument plugin to extract a civicrm user id via menu_get_object
 */
class civicrm_plugin_argument_default extends views_plugin_argument_default {
  public $option_name = 'default_argument_civicrm';

  public function argument_form(&$form, &$form_state) {
    $form[$this->option_name] = array(
      '#type' => 'checkbox',
      '#title' => t('Also look for a node and use the node author'),
      '#default_value' => !empty($this->argument->options[$this->option_name]),
      '#process' => array('civicrm_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id),
      ),
      '#dependency_count' => 2,
    );
  }

  public function get_argument() {
    foreach (range(1, 3) as $i) {
      $user = menu_get_object('user', $i);
      if (!empty($user)) {
        return $user->uid;
      }
    }

    foreach (range(1, 3) as $i) {
      $user = menu_get_object('civicrm_uid_optional', $i);
      if (!empty($user)) {
        return $user->uid;
      }
    }

    if (!empty($this->argument->options[$this->option_name])) {
      foreach (range(1, 3) as $i) {
        $node = menu_get_object('node', $i);
        if (!empty($node)) {
          return $node->uid;
        }
      }
    }
  }

}
