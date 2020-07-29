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
 * Build the $data array for CiviLineItem related tables
 * Includes the following tables
 * civicrm_line_item
 */
function _civicrm_lineitem_data(&$data, $enabled) {
  //------------------------------------------------------
  // CIVICRM Line Items are here with all the connections.
  //------------------------------------------------------

  $data['civicrm_line_item']['table']['group'] = t('CiviCRM Line-items');

  $data['civicrm_line_item']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Line-items'),
    'help' => t("View displays CiviCRM Line-items."),
  );

  //CiviCRM Line-items - FIELDS

  // Link to contribution table
  if (isset($enabled['CiviContribute'])) {
    $data['civicrm_line_item']['contribution_id'] = array(
      'title' => t('Contributions Records'),
      'help' => 'Bring contributions records',
      'relationship' => array(
        'base' => 'civicrm_contribution',
        'base field' => 'id',
        'handler' => 'views_handler_relationship',
        'label' => t('line-item -> Contribution'),
      ),
    );
  }

  // Line-item ID field
  $data['civicrm_line_item']['id'] = array(
    'title' => t('ID'),
    'help' => t("The numeric Line-item's ID."),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Line-item label
  $data['civicrm_line_item']['label'] = array(
    'title' => t('Label'),
    'help' => t('The line item label, display the formatted amount'),
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

  // Line-item - price field relationship
  $data['civicrm_line_item']['price_field_id'] = array(
    'title' => t('Price Field'),
    'help' => t('Price Field for this line item'),
    'real field' => 'price_field_id',
    'relationship' => array(
      'base' => 'civicrm_price_field',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Price Field'),
    ),
  );

  // Line-item - price field value relationship
  $data['civicrm_line_item']['price_field_value_id'] = array(
    'title' => t('Price Field Value'),
    'help' => t('Price Field Value for this line item'),
    'real field' => 'price_field_value_id',
    'relationship' => array(
      'base' => 'civicrm_price_field_value',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Price Field Value'),
    ),
  );

  // Line-item - contribution relationship
  $data['civicrm_line_item']['entity_id'] = array(
    'title' => t('Contribution Record ID'),
    'help' => t('Bring Contribution Record the complete contribution record to the party'),
    'relationship' => array(
      'base' => 'civicrm_contribution',
      'left_field' => 'id',
      'field' => 'entity_id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contribution, with custom fields'),
    ),
  );

  // Line-Item Total amount
  $data['civicrm_line_item']['line_total'] = array(
    'title' => t('Total Line Item'),
    'help' => t('The Total for the line item'),
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
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Line-Item unit price
  $data['civicrm_line_item']['unit_price'] = array(
    'title' => t('Unit Price Item'),
    'help' => t('The unit price for this line-item'),
    'field' => array(
      'handler' => 'civicrm_handler_field_money',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Line-Item quantity
  $data['civicrm_line_item']['qty'] = array(
    'title' => t('Item quantity'),
    'help' => t('The quantity for this line-item'),
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
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Line-item financial type
  $data['civicrm_line_item']['financial_type'] = array(
    'title' => t('Line item Financial Type'),
    'help' => t('Financial type for Line Item, ie Member Dues, Event Fees etc'),
    'real field' => 'financial_type_id',
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'financialType',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Contribute_PseudoConstant',
      'pseudo method' => 'financialType',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

}
