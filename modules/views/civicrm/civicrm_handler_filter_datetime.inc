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

/*
 * Unchanged anonymous code contribution. No claim.
 * CiviCRM Views Integration
 *
 * civicrm_handler_filter_datetime.inc
 *
 */

/**
 * All the comparisons need to be done with SQL DATETIMES and not unixtime
 */
class civicrm_handler_filter_datetime extends views_handler_filter_date {

  /**
   * We use strtotime() to accept a wide range of date inputs and then
   * convert the unixtime back to SQL DATETIME before adding the WHERE clause
   */
  public function op_simple($field) {
    $value = intval(strtotime($this->value['value'], 0));

    if (!empty($this->value['type']) && $this->value['type'] == 'offset') {
      // keep sign
      $value = time() + sprintf('%+d', $value);
    }

    $value = $this->format_date($value);

    $this->query->add_where($this->options['group'], $field, $value, $this->operator);
  }

  public function op_between($field) {
    $a = intval(strtotime($this->value['min'], 0));
    $b = intval(strtotime($this->value['max'], 0));

    if ($this->value['type'] == 'offset') {
      $now = time();
      // keep sign
      $a = $now + sprintf('%+d', $a);
      // keep sign
      $b = $now + sprintf('%+d', $b);
    }

    $a = $this->format_date($a);
    $b = $this->format_date($b);

    // This is safe because we are manually scrubbing the values.
    // It is necessary to do it this way because $a and $b are formulas when using an offset.
    $operator = strtoupper($this->operator);
    $this->query->add_where_expression($this->options['group'], "$field $operator '$a' AND '$b'");
  }

  public function format_date($unixtime) {
    return date("Y-m-d H:i:s", $unixtime);
  }

}
