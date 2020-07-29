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
 * Argument handler for a day (DD)
 */
class views_handler_argument_civicrm_day extends views_handler_argument_date {

  /**
   * Constructor implementation
   */
  public function construct() {
    parent::construct();
    $this->format     = 'j';
    $this->arg_format = 'd';
    $this->formula    = views_date_sql_extract('DAY', "***table***.$this->real_field", "datetime", 0);
  }

  /**
   * Provide a link to the next level of the view
   */
  public function summary_name($data) {
    $day = str_pad($data->{$this->name_alias}, 2, '0', STR_PAD_LEFT);
    return format_date(strtotime("2005" . "05" . $day . " 00:00:00 UTC"), 'custom', $this->format, 'UTC');
  }

  /**
   * Provide a link to the next level of the view
   */
  public function title() {
    $day = str_pad($this->argument, 2, '0', STR_PAD_LEFT);
    return format_date(strtotime("2005" . "05" . $day . " 00:00:00 UTC"), 'custom', $this->format, 'UTC');
  }

  public function summary_argument($data) {
    // Make sure the argument contains leading zeroes.
    return str_pad($data->{$this->base_alias}, 2, '0', STR_PAD_LEFT);
  }

}
