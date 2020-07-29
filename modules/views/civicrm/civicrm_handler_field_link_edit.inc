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
 * Field handler to present a link to user edit.
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_link_edit extends civicrm_handler_field_link {

  /**
   * An example of field level access control.
   */
  public function access() {
    return user_access('access CiviCRM');
  }

  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('edit');
    $id = $values->id;
    return civicrm_views_href($text,
      'civicrm/contact/add',
      "reset=1&action=update&cid=$id"
    );
  }

}
