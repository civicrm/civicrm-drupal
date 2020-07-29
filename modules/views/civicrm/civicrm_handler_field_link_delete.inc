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
 * Field handler to present a link to user delete.
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_link_delete extends civicrm_handler_field_link {

  /**
   * An example of field level access control.
   */
  public function access() {
    return user_access('access CiviCRM');
  }

  public function render($values) {
    $text = !empty($this->options['text']) ? $this->options['text'] : t('delete');
    $id = $values->{$this->aliases['id']};
    return civicrm_views_href($text,
      'civicrm/contact/delete',
      "reset=1&id=$id"
    );
  }

}
