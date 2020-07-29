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
 * Field hanlder to allow filtering by CiviCRM domain.
 */
class civicrm_handler_filter_domain extends views_handler_filter_numeric {

  /**
   * Add an additonal operator which takes no arguments to the existing list provided by views_handler_filter_numeric.
   */
  public function operators() {
    $operators = parent::operators();
    $operators['current_domain'] = array(
      'title' => t('Current CiviCRM domain'),
      'method' => 'current_domain',
      'short' => t('current domain'),
      'values' => 0,
    );
    return $operators;
  }

  /**
   * Grab the current domain id using the civicrm api and filter the query.
   */
  public function current_domain($field) {
    if (!civicrm_initialize()) {
      return FALSE;
    }

    $domain = civicrm_api('domain', 'getsingle', array(
      'current_domain' => 1,
      'version' => 3,
    ));

    if (!$domain['is_error']) {
      $this->query->add_where($this->options['group'], $field, $domain['id']);
    }
  }

}
