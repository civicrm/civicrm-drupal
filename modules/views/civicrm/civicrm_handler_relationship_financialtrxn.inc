<?php

/**
 * @file
 * Relationship handler for links financial transactions to entities via civicrm_entity_financial_trxn.
 */

class civicrm_handler_relationship_financialtrxn extends views_handler_relationship {
  function query() {
    // Figure out what base table this relationship brings to the party.
    $table_data = views_fetch_data($this->definition['base']);
    $base_field = empty($this->definition['base field']) ? $table_data['table']['base']['field'] : $this->definition['base field'];

    $this->ensure_my_table();

    // First JOIN is to the civicrm_entity_financial_trxn table.
    
    $def_l = $this->definition;
    $def_l['table'] = 'civicrm_entity_financial_trxn';
    $def_l['field'] = 'financial_trxn_id';
    $def_l['left_table'] = $this->table_alias;
    $def_l['left_field'] = $this->real_field;
    if (!empty($this->options['required'])) {
      $def_l['type'] = 'INNER';
    }

    $join_l = new views_join();

    $join_l->definition = $def_l;
    $join_l->options = $this->options;
    $join_l->construct();
    $join_l->adjusted = TRUE;

    // use a short alias:
    
    $alias = $def_l['table'] . '_' . $this->table;

    $alias_l = $this->query->add_relationship($alias, $join_l, 'civicrm_entity_financial_trxn', $this->relationship);

    // Second JOIN is to the entity (base) table defined by the relationship.
    
    $def_r = $this->definition;
    $def_r['table'] = $this->definition['base'];
    $def_r['field'] = $base_field;
    $def_r['left_table'] = $alias_l;
    $def_r['left_field'] = 'entity_id';
    if (!empty($this->options['required'])) {
      $def_r['type'] = 'INNER';
    }

    if (!empty($this->definition['extra'])) {
      $def_r['extra'] = $this->definition['extra'];
    }

    // Only join when the entity_table matches the base table defined in the relationship.
    
    $def_r['extra'][] = array(
      'table' => $alias_l,
      'field' => 'entity_table',
      'value' => $this->definition['base'],
    );

    $join_r = new views_join();

    $join_r->definition = $def_r;
    $join_r->options = $this->options;
    $join_r->construct();
    $join_r->adjusted = TRUE;
    
    $alias = $def_r['table'] . '_' . $this->table;

    $this->alias = $this->query->add_relationship($alias, $join_r, $this->definition['base'], $this->relationship);

    // Add access tags if the base table provides it.
    if (empty($this->query->options['disable_sql_rewrite']) && isset($table_data['table']['base']['access query tag'])) {
      $access_tag = $table_data['table']['base']['access query tag'];
      $this->query->add_tag($access_tag);
    }
  }
}