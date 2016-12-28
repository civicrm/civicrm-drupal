<?php

/**
 * This relationship handler is used when joining the civicrm_group_contact table
* to the civicrm_contact table. This handler allows us to optionally add conditions
* to the join clause based on group_id, status, group title, is_active, group is standard group.
*/
class civicrm_handler_relationship_standard_group extends civicrm_handler_relationship_group {

  function construct() {
    $this->is_standard = true;
    parent::construct();
  }
}