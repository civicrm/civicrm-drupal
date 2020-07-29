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
 * Field handler to provide relationship to CiviCRM email.
 *
 * @ingroup civicrm_relationship_handlers
 */
class civicrm_handler_relationship_mail extends civicrm_handler_relationship_location {

  /**
   * Called to implement a relationship in a query.
   */
  public function query() {
    $join = $this->get_join();
    $this->alias = $this->query->add_table($this->table, $this->relationship, $join);
    //register relationship
    $this->view->relationship[$this->options['id']]->alias = $this->alias;
    $this->query->relationships[$this->alias] = array(
      'link' => $this->relationship,
      'table' => $this->table,
      'base' => $this->table,
    );
  }

}
