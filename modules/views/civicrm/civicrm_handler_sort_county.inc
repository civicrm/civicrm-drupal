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
 * sort handler to properly pass the field type for date fields.
 *
 */
class civicrm_handler_sort_county extends civicrm_handler_sort_address_pseudoconstant {

  public function ensure_my_table() {
    $this->_join_table = 'civicrm_county';
    $this->_join_left_field = 'county_id';
    $this->_join_field = 'id';

    return parent::ensure_my_table();
  }

}
