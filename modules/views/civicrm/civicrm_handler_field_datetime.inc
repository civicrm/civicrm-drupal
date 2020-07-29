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
 * Unchanged anonymous code contribution. No claim.
 *
 * civicrm_handler_field_datetime.inc
 * Converts and displays text to something views friendly.
 *
 */
class civicrm_handler_field_datetime extends views_handler_field_date {

  /**
   * Convert the DATETIME from the database into unixtime then allow
   * views_handler_field_date to render as usual.
   * Also trick php into thinking the time is in the same timezone, no
   * matter the default timezone
   */
  public function render($values) {
    $value = $values->{$this->field_alias};

    if (is_string($value) && strpos($value, "-")) {
      $value = strtotime($value);

      if ($value) {
        $values->{$this->field_alias} = $value;
      }
    }

    return parent::render($values);
  }

}
