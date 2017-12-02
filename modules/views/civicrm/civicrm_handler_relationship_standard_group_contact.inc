<?php

/**
 * This relationship handler is used when joining the civicrm_group_contact table
 * to the civicrm_contact table. This handler allows us to optionally add conditions
 * to the join clause based on group_id, status, group title, is_active, group is standard group.
 */
class civicrm_handler_relationship_standard_group_contact extends views_handler_relationship {
  static $group_status;
  private $group_ids = array();
  private $group_titles = array();

  /**
   * Preload the list of group_types & titles and store in the static variables
   */
  public function construct() {
    parent::construct();

    if (!civicrm_initialize()) {
      return;
    }

    if (empty(self::$group_status)) {
      self::$group_status = CRM_Contact_BAO_GroupContact::buildOptions('status');
    }

    $result = civicrm_api3('Group', 'get', array(
        'return' => array("name", "title"),
        'is_hidden' => 0,
        'is_active' => 1,
        'saved_search_id' => array('IS NULL' => 1),
        'options' => array('limit' => 0, 'sort' => "title"),
    ));
    foreach ($result['values'] as $group) {
      if (isset($group['name'])) {
        $this->group_ids[$group['name']] = $group['id'];
        $this->group_titles[$group['name']] = $group['title'];
      }
    }
  }

  /**
   * Add additional options
   * to the view. By defining these here, Views will take care of saving the
   * values submitted from the options form.
   */
  public function option_definition() {
    $options = parent::option_definition();
    $options['group_status'] = array('default' => 'Added');
    $options['group_names'] = array('default' => NULL);
    return $options;
  }

  /**
   * Relationship configuration form.
   */
  public function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['group_status'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => 'Choose a specific group status',
      '#options' => self::$group_status,
      '#description' => t('Choose to limit this relationship to one or more specific status of CiviCRM group.'),
      '#default_value' => $this->options['group_status'],
    );
    $form['group_names'] = array(
      '#type' => 'select',
      '#multiple' => TRUE,
      '#title' => 'Choose a specific group',
      '#options' => $this->group_titles,
      '#description' => t('Choose to limit this relationship to one or more specific CiviCRM groups.'),
      '#default_value' => $this->options['group_names'],
    );
  }

  public function join_required($join = array()) {
    if (!empty($this->options['required'])) {
      $join->type = 'INNER';
    }
    return $join;
  }

  public function join_group_contact($join = array()) {
    $extra = array();
    if (isset($join->extra)) {
      $extra = $join->extra;
    }
    if (!empty($this->options['group_status'])) {
      $extra[] = array(
        'value' => $this->options['group_status'],
        'numeric' => FALSE,
        'field' => 'status',
      );
    }
    if (!empty($this->options['group_names'])) {
      $values = array();
      foreach (array_keys($this->options['group_names']) as $name) {
        $values[] = $this->group_ids[$name];
      }
      $extra[] = array(
        'value' => $values,
        'numeric' => TRUE,
        'field' => 'group_id',
      );
    }
    if (!empty($extra)) {
      $join->extra = $extra;
    }
    return $join;
  }

  public function get_join() {
    $join = parent::get_join();
    $join = $this->join_required($join);
    $join = $this->join_group_contact($join);
    return $join;
  }

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
