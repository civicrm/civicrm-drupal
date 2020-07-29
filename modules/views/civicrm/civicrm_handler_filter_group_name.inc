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
 * CiviCRM Views Integration
 */
class civicrm_handler_filter_group_name extends views_handler_filter_in_operator {
  private $civi_groups;
  protected $is_standard = TRUE;

  public function construct() {
    parent::construct();

    if (empty($this->civi_groups)) {
      if (!civicrm_initialize()) {
        return;
      }
      $standard_or_smart = ($this->is_standard ? 'IS NULL' : 'IS NOT NULL');
      $result = civicrm_api3('Group', 'get', [
        'return' => ["name", "title"],
        'is_hidden' => 0,
        'is_active' => 1,
        'saved_search_id' => [$standard_or_smart => 1],
        'options' => ['limit' => 0, 'sort' => "title"],
      ]);
      $this->civi_groups = [];
      foreach ($result['values'] as $group) {
        if (isset($group['name'])) {
          $this->civi_groups[$group['name']] = $group['title'];
        }
      }
    }
  }

  public function get_value_options() {
    if (!isset($this->value_options)) {
      $this->value_title = t('Contact Group Title');
      $options = [];
      foreach ($this->civi_groups as $name => $title) {
        $options[$name] = $title;
      }
      $this->value_options = $options;
    }
  }

}
