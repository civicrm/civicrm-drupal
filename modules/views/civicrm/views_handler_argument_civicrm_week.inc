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
 * Argument handler for a week.
 */
class views_handler_argument_civicrm_week extends views_handler_argument_date {

  /**
   * Constructor implementation
   */
  public function construct() {
    parent::construct();
    $this->arg_format = 'w';
    $this->formula = views_date_sql_extract('WEEK', "***table***.$this->real_field", "datetime", '0');
  }

  /**
   * Provide a link to the next level of the view
   */
  public function summary_name($data) {
    $created = $data->{$this->name_alias};
    return t('Week %week', array('%week' => $created));
  }

}
