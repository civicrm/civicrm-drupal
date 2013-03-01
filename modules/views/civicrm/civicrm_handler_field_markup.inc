<?php
// $Id$


/**
 * A handler to run a field through check_markup, using the default format
 *
 */
class civicrm_handler_field_markup extends views_handler_field {
  function construct() {
    parent::construct();
  }

  function render($values) {
    $value = $values->{$this->field_alias};
    $retval = check_markup($value, FILTER_FORMAT_DEFAULT, FALSE);
    return ($retval == 'n/a') ? NULL : $retval;
  }
}

