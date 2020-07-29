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
 * Argument handler for a year plus month (CCYYMM)
 */
class views_handler_argument_civicrm_year_month extends views_handler_argument_date {

  /**
   * Constructor implementation
   */
  public function construct() {
    parent::construct();
    $this->format     = 'F Y';
    $this->arg_format = 'Ym';
    $this->formula    = views_date_sql_format($this->arg_format, "***table***.$this->real_field", "datetime", 0);
  }

  /**
   * Provide a link to the next level of the view
   */
  public function summary_name($data) {
    $timestamp = $data->{$this->name_alias};
    $timestamp = strtotime($timestamp . " UTC");
    if ($timestamp !== FALSE) {
      return format_date($timestamp, 'custom', $this->format, 'UTC');
    }
  }

  /**
   * Provide a link to the next level of the view
   */
  public function title() {
    $timestamp = strtotime($this->argument . " UTC");
    if ($timestamp !== FALSE) {
      return format_date($timestamp, 'custom', $this->format, 'UTC');
    }
  }

}
