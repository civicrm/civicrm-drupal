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
 * Argument handler for a year (CCYY)
 */
class views_handler_argument_civicrm_year extends views_handler_argument_date {

  /**
   * Constructor implementation
   */
  public function construct() {
    parent::construct();
    $this->arg_format = 'Y';
    $this->formula = views_date_sql_format('Y', "***table***.$this->real_field", "datetime", 0);
    //$this->formula = views_date_sql_extract('YEAR', "***table***.$this->real_field");
    // Any reason the above was used?
  }

}
