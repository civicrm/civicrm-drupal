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
 * Build the $data array for CiviGrant related tables
 * Includes the following tables
 * civicrm_grant
 */
function _civicrm_grant_data(&$data, $enabled) {

  /**
   * CiviCRM Grants Base Table
   */

  $data['civicrm_grant']['table']['group'] = t('CiviCRM Grants');

  $data['civicrm_grant']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Grants'),
    'help' => t("View displays CiviCRM Grants, with relation to contacts."),
  );

  ///CiviCRM Grants - JOINS

  $data['civicrm_grant']['table']['join'] = array(
    // Directly links to contact table
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );
  // Display Grants in User Views
  $data['civicrm_grant']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );

  //CiviCRM Grants - FIELDS

  //Numeric Grant ID
  $data['civicrm_grant']['id'] = array(
    'title' => t('Grant ID'),
    'help' => t('The numeric ID of the Grant'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Status
  $data['civicrm_grant']['grant_status'] = array(
    'title' => t('Grant Status'),
    'real field' => 'status_id',
    'help' => t('The Status of this Grant'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Grant_DAO_Grant',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Grant_DAO_Grant',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Application Received Date
  $data['civicrm_grant']['application_received_date'] = array(
    'title' => t('Application Received Date'),
    'help' => t('Date the Grant application was received'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_grant'], array('title' => 'Application Received Date', 'name' => 'application_received_date'));

  //Grant Decision Date
  $data['civicrm_grant']['decision_date'] = array(
    'title' => t('Decision Date'),
    'help' => t('Date on which Grant decision was made'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_grant'], array('title' => 'Decision Date', 'name' => 'decision_date'));

  //Grant Money Transfer Date
  $data['civicrm_grant']['money_transfer_date'] = array(
    'title' => t('Money Transfer Date'),
    'help' => t('Date on which Grant money transfer was made'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_grant'], array('title' => 'Money Transfer Date', 'name' => 'money_transfer_date'));

  //Grant Report Due Date
  $data['civicrm_grant']['grant_due_date'] = array(
    'title' => t('Report Due Date'),
    'help' => t('Date on which Grant report is due'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_grant'], array('title' => 'Report Due Date', 'name' => 'grant_due_date'));

  //Grant Report Due Date
  $data['civicrm_grant']['grant_due_date'] = array(
    'title' => t('Report Due Date'),
    'help' => t('Date on which Grant report is due'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_grant'], array('title' => 'Report Due Date', 'name' => 'grant_due_date'));

  //Grant Report Received (boolean)
  $data['civicrm_grant']['grant_report_received'] = array(
    'title' => t('Grant Report Received'),
    'help' => t('Has the Grant Report been received?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Type
  $data['civicrm_grant']['grant_type'] = array(
    'title' => t('Grant Type'),
    'real field' => 'grant_type_id',
    'help' => t('The Type of this Grant.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Grant_DAO_Grant',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Grant_DAO_Grant',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Amount Requested
  $data['civicrm_grant']['amount_total'] = array(
    'title' => t('Amount Requested'),
    'help' => t('Grant amount requested'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
      'currency field' => 'currency',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Amount Requested (origninal currency)
  $data['civicrm_grant']['amount_requested'] = array(
    'title' => t('Amount Requested (origninal currency)'),
    'help' => t('Amount requested for grant in original currency (if different)'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Amount Granted
  $data['civicrm_grant']['amount_granted'] = array(
    'title' => t('Amount Granted'),
    'help' => t('Grant amount awarded'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
      'currency field' => 'currency',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Rationale
  $data['civicrm_grant']['rationale'] = array(
    'title' => t('Rationale'),
    'help' => t('Grant rationale'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
}
