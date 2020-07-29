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
 * Argument handler for a full date (CCYYMMDD)
 */
class views_handler_argument_civicrm_fulldate extends views_handler_argument_date {

  /**
   * Constructor implementation
   */
  public function construct() {
    parent::construct();
    $this->format     = 'F j, Y';
    $this->arg_format = 'Ymd';
    $this->formula    = views_date_sql_format($this->arg_format, "***table***.$this->real_field", "datetime");
    //$this->formula = views_date_sql_extract($this->arg_format, "***table***.$this->real_field",'datetime',0);
    // Any reason the above was used?
  }

  /**
   * Provide a link to the next level of the view
   */
  public function summary_name($data) {
    $created = $data->{$this->name_alias};
    return format_date(strtotime($created . " 00:00:00 UTC"), 'custom', $this->format, 'UTC');
  }

  /**
   * Provide a link to the next level of the view
   */
  public function title() {
    return format_date(strtotime($this->argument . " 00:00:00 UTC"), 'custom', $this->format, 'UTC');
  }

}
