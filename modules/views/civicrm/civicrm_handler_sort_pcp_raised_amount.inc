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
 * sorting handler to calcuate the total amount raised for PCPs.
 *
 */
class civicrm_handler_sort_pcp_raised_amount extends views_handler_sort {

  public function query() {
    $this->ensure_my_table();

    // @see CRM_PCP_BAO_PCP::thermoMeter for the source of this SQL
    $formula = "
(SELECT SUM(cc.total_amount) as total
FROM {civicrm_pcp} pcp2
LEFT JOIN {civicrm_contribution_soft} cs ON ( pcp2.id = cs.pcp_id )
LEFT JOIN {civicrm_contribution} cc ON ( cs.contribution_id = cc.id)
WHERE pcp2.id = " . $this->table_alias . "." . $this->real_field . " AND cc.contribution_status_id =1 AND cc.is_test = 0)";

    $this->query->add_orderby(NULL, $formula, $this->options['order'], $this->table_alias . '_' . $this->field);
  }

}
