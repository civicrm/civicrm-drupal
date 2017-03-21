<?php

/**
 * This relationship handler is used when joining the civicrm_group_contact table
* to the civicrm_contact table. This handler allows us to optionally add conditions
* to the join clause based on group_id, status, group title, is_active, group is standard group.
*/
class civicrm_handler_relationship_group extends views_handler_relationship {
  static $civi_group_types;
  private $civi_groups;
  protected $is_standard;

  /**
   * Preload the list of group_types & titles and store in the static variables
   */
  function construct() {
    parent::construct();

    if (!civicrm_initialize()) {
      return;
    }
    require_once 'CRM/Contact/BAO/Group.php';
    self::$civi_group_types = CRM_Contact_BAO_Group::buildOptions('group_type');

    $standard_or_smart = ($this->is_standard ? 'IS NULL' : 'IS NOT NULL');
    $result = civicrm_api3('Group', 'get', array(
        'return' => array("name", "title", "group_type"),
        'is_hidden' => 0,
        'is_active' => 1,
        'saved_search_id' => array($standard_or_smart => 1),
        'options' => array('limit' => 0, 'sort' => "title"),
    ));
    $this->civi_groups = array();
    foreach ($result['values'] as $group) {
      $group_types = array();
      if (!empty($group['group_type'])) {
        if (!is_array($group['group_type'])) {
          $group['group_type'] = array($group['group_type']);
        }
        foreach($group['group_type'] as $type) {
          if (!empty($type))
            $group_types[] = (isset(self::$civi_group_types[$type]) ? self::$civi_group_types[$type] : $type);
        }
      }
      $group_type = implode(', ', $group_types);

      if (isset($group['name'])) {
        $this->civi_groups[$group['name']] =
          $group['title'] . ' | ' .
          $group_type;
      }
    }
  }


  /**
   * Add additional options
   * to the view. By defining these here, Views will take care of saving the
   * values submitted from the options form.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['civi_group_type'] = array('default' => NULL);
    $options['civi_group_name'] = array('default' => NULL);
    return $options;
  }

  /**
   * Relationship configuration form.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['civi_group_type'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => 'Choose a specific group type',
        '#options' => self::$civi_group_types,
        '#description' => t('Choose to limit this relationship to one or more specific types of CiviCRM group.'),
        '#default_value' => $this->options['civi_group_type'],
    );
    $form['civi_group_name'] = array(
        '#type' => 'select',
        '#multiple' => TRUE,
        '#title' => 'Choose a specific group',
        '#options' => $this->civi_groups,
        '#description' => t('Choose to limit this relationship to one or more specific CiviCRM groups.'),
        '#default_value' => $this->options['civi_group_name'],
    );
  }


  function join_required($join = array()) {
    if (!empty($this->options['required'])) {
      $join->type = 'INNER';
    }
    return $join;
  }


  function join_group_type($join = array()) {
    $extra = array();
    if (isset($join->extra)) {
      $extra = $join->extra;
    }
    if (isset($this->options['civi_group_type']) && $this->options['civi_group_type']) {
      $sep = CRM_Core_DAO::VALUE_SEPARATOR;

      $extra[] = array(
          'value' =>
          "($sep" .
          implode("$sep|$sep", $this->options['civi_group_type']) .
          "$sep)",
          'numeric' => FALSE,
          'field' => 'group_type',
          'operator' => 'RLIKE',
      );
    }
    if (!empty($extra)) {
      $join->extra = $extra;
    }
    return $join;
  }

  function join_group_name($join = array()) {
    $extra = array();
    if (isset($join->extra)) {
      $extra = $join->extra;
    }
    if (isset($this->options['civi_group_name']) && $this->options['civi_group_name']) {
      $extra[] = array(
          'value' => $this->options['civi_group_name'],
          'numeric' => FALSE,
          'field' => 'name',
      );
    }
    if (!empty($extra)) {
      $join->extra = $extra;
    }
    return $join;
  }


  function join_group_active_not_hidden($join = array()) {
    $extra = array();
    if (isset($join->extra)) {
      $extra = $join->extra;
    }
    $extra[] = array(
      'value' => 1,
      'numeric' => TRUE,
      'field' => 'is_active',
    );
    $extra[] = array(
      'value' => 0,
      'numeric' => TRUE,
      'field' => 'is_hidden',
    );

    $join->extra = $extra;
    return $join;
  }

  function get_join() {
    $join = parent::get_join();
    $join = $this->join_group_active_not_hidden($join);
    $join = $this->join_required($join);
    $join = $this->join_group_type($join);
    $join = $this->join_group_name($join);
    return $join;
  }


  /**
   * Called to implement a relationship in a query.
   */
  function query() {
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