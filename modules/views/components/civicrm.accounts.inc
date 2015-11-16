<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.6                                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/

/**
 * Build the $data array for CiviAccounts tables
 * Includes the following tables
 * civicrm_financial_trxn
 * civicrm_entity_financial_trxn
 * civicrm_financial_item
 * civicrm_financial_account
 */
function _civicrm_accounts_data(&$data, $enabled) {

  /**
   * CiviCRM Financial Transaction Base Table.
   */

  $data['civicrm_financial_trxn']['table']['group'] = t('CiviCRM Financial Transactions');

  $data['civicrm_financial_trxn']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Financial Transaction'),
    'help'  => t('View displays CiviCRM Financial Transactions.'),
  );

  // CiviAccounts Transaction ID.

  $data['civicrm_financial_trxn']['id'] = array(
    'title' => t('ID'),
    'help' => t('The numeric ID of the Transaction'),
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

  // Financial Account IDs.

  $data['civicrm_financial_trxn']['from_financial_account_id'] = array(
    'title' => t('Financial Transaction From Account ID'),
    'help' => t('The numeric ID of the From Account'),
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
    'relationship' => array (
      'base' => 'civicrm_financial_account',
      'base field' => 'id',
      'label' => t('From Financial Account'),
    ),
  );

  $data['civicrm_financial_trxn']['to_financial_account_id'] = array(
    'title' => t('Financial Transaction To Account ID'),
    'help' => t('The numeric ID of the To Account'),
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
    'relationship' => array (
      'base' => 'civicrm_financial_account',
      'base field' => 'id',
      'label' => t('To Financial Account'),
    ),
  );

  // Transaction Date.

  $data['civicrm_financial_trxn']['trxn_date'] = array(
    'title' => t('Transaction Date'),
    'help' => t('Date the Transaction occurred'),
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
  civicrm_views_add_date_arguments($data['civicrm_financial_trxn'], array('title' => 'Transaction Date', 'name' => 'trxn_date'));

  // Transaction Total.

  $data['civicrm_financial_trxn']['total_amount'] = array(
    'title' => t('Total Amount'),
    'help' => t('The Total Contribution made by the Contact'),
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

  // Fee Amount.

  $data['civicrm_financial_trxn']['fee_amount'] = array(
    'title' => t('Fee Amount'),
    'help' => t('The fee charged by the Payment Processor for the transaction.'),
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
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Net Amount.

  $data['civicrm_financial_trxn']['net_amount'] = array(
    'title' => t('Net Amount'),
    'help' => t('The Full Contribution minus Payment Processor Fees.'),
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
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Currency.

  $data['civicrm_financial_trxn']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency used for the Transaction'),
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

  // External Transaction ID.

  $data['civicrm_financial_trxn']['trxn_id'] = array(
    'title' => t('Transaction ID'),
    'help' => t('Unique Transaction ID from Payment Processor / Bank etc'),
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

  // Transaction Result Code.

  $data['civicrm_financial_trxn']['trxn_result_code'] = array(
    'title' => t('Result Code'),
    'help' => t('Transaction Result Code'),
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

  // Status ID.

  $data['civicrm_financial_trxn']['status_id'] = array(
    'title' => t('Financial Transaction Status ID'),
    'help' => t('The numeric ID of the Financial Transaction Status'),
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

  // Payment Processor ID.

  $data['civicrm_financial_trxn']['payment_processor_id'] = array(
    'title' => t('Payment Processor ID'),
    'help' => t('The numeric ID of the Payment Processor'),
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

  // Payment Instrument ID.

  $data['civicrm_financial_trxn']['payment_instrument_id'] = array(
    'title' => t('Payment Instrument ID'),
    'help' => t('The numeric ID of the Payment Instrument'),
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

  // Check Number.

  $data['civicrm_financial_trxn']['check_number'] = array(
    'title' => t('Check Number'),
    'help' => t('Unique Check Number'),
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

  /**
   * Define relationships between Contributions and their Financial Transactions
   */

  $data['civicrm_entity_financial_trxn']['table']['join']['civicrm_financial_trxn'] = array (
    'left_table' => 'civicrm_financial_trxn',
    'left_field' => 'id',
    'field' => 'financial_trxn_id',
    'type' => 'left',
  );

  $data['civicrm_financial_trxn']['table']['join']['civicrm_entity_financial_trxn'] = array (
    'left_table' => 'civicrm_entity_financial_trxn',
    'left_field' => 'financial_trxn_id',
    'field' => 'id',
    'type' => 'left',
  );

  // Relate Contribution via Transaction Entity.

  $data['civicrm_financial_trxn']['trxn_contribution'] = array (
    'title' => t('Contribution from Transaction'),
    'help' => t('Connects a financial transaction to a contribution'),
    'relationship' => array (
      'base' => 'civicrm_contribution',
      'base field' => 'id',
      'relationship table' => 'civicrm_financial_trxn',
      'relationship field' => 'id',
      'handler' => 'civicrm_handler_relationship_financialtrxn',
      'label' => t('Contribution Entity from Transaction'),
    ),
  );

  // Relate Financial Item via Transaction Entity.

  $data['civicrm_financial_trxn']['financial_item'] = array (
    'title' => t('Financial Item from Transaction'),
    'help' => t('Connects a financial transaction to a financial item'),
    'relationship' => array (
      'base' => 'civicrm_financial_item',
      'base field' => 'id',
      'relationship table' => 'civicrm_financial_trxn',
      'relationship field' => 'id',
      'handler' => 'civicrm_handler_relationship_financialtrxn',
      'label' => t('CiviCRM Financial Item from Transaction'),
    ),
  );

  $data['civicrm_contribution']['financial_trxn'] = array (
    'real field' => 'id',
    'title' => t('Financial Transaction from Contribution'),
    'help' => t('Connects a contribution to a financial transaction'),
    'relationship' => array (
      'base' => 'civicrm_entity_financial_trxn',
      'base field' => 'entity_id',
      'extra' => array (
        array(
          'field' => 'entity_table',
          'value' => 'civicrm_contribution',
        ),
      ),
      'handler' => 'views_handler_relationship',
      'label' => t('Financial Transaction from Contribution'),
    ),
  );

  /**
   * Base table information for linking civicrm_entity_financial_trxn.
   * Links Financial Transactions to Items or Contributions.
   */

  $data['civicrm_entity_financial_trxn']['table']['group'] = t('CiviCRM Entity Financial Transactions');

  // Link ID.

  $data['civicrm_entity_financial_trxn']['id'] = array(
    'title' => t('Transaction Entity ID'),
    'help' => t('The numeric ID of the Transaction Entity'),
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

  // Entity Type / Table.

  $data['civicrm_entity_financial_trxn']['entity_table'] = array(
    'title' => t('Entity Table'),
    'help' => t('Entity Table'),
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

  // Contribution / Financial Item ID.

  $data['civicrm_entity_financial_trxn']['entity_id'] = array(
    'title' => t('Entity ID'),
    'help' => t('The numeric ID of the Entity'),
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

  // Transaction ID.

  $data['civicrm_entity_financial_trxn']['financial_trxn_id'] = array(
    'title' => t('Financial Transaction ID'),
    'help' => t('The numeric ID of the Financial Transaction'),
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

  // Entity amount.

  $data['civicrm_entity_financial_trxn']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The Amount of the Transaction Entity'),
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

  /**
   * CiviCRM Financial Item Table
   */

  $data['civicrm_financial_item']['table']['group'] = t('CiviCRM Financial Items');

  $data['civicrm_financial_item']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Financial Items'),
    'help'  => t('View displays CiviCRM Financial Items.'),
  );

  // Financial Item ID.

  $data['civicrm_financial_item']['id'] = array(
    'title' => t('Financial Item ID'),
    'help' => t('The numeric ID of the Financial Item'),
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

  // Created Date.

  $data['civicrm_financial_item']['created_date'] = array(
    'title' => t('Financial Item Created Date'),
    'help' => t('Date the Financial Item was created'),
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
  civicrm_views_add_date_arguments($data['civicrm_financial_item'], array('title' => 'Financial Item Date', 'name' => 'created_date'));

  // Transaction Date.

  $data['civicrm_financial_item']['transaction_date'] = array(
    'title' => t('Financial Item Transaction Date'),
    'help' => t('Date the Transaction occurred'),
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
  civicrm_views_add_date_arguments($data['civicrm_financial_item'], array('title' => 'Transaction Date', 'name' => 'transaction_date'));

  // Contact ID.

  $data['civicrm_financial_item']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The numeric ID of the Contact'),
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

  // Description.

  $data['civicrm_financial_item']['description'] = array(
    'title' => t('Description'),
    'help' => t('The Description of the Financial Item'),
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

  // Amount.

  $data['civicrm_financial_item']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The Total Amount of the Financial Item'),
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

  // Currency.

  $data['civicrm_financial_item']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency used for the Financial Item'),
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

  // Financial Account.

  $data['civicrm_financial_item']['financial_account_id'] = array(
    'title' => t('Financial Account ID'),
    'help' => t('The numeric ID of the Financial Item\'s Account'),
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
    'relationship' => array (
      'base' => 'civicrm_financial_account',
      'base field' => 'id',
      'label' => t('Financial Account'),
    )
  );

  // Status ID.

  $data['civicrm_financial_item']['status_id'] = array(
    'title' => t('Financial Item Status ID'),
    'help' => t('The numeric ID of the Financial Item Status'),
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

  // Entity Table.

  $data['civicrm_financial_item']['entity_table'] = array(
    'title' => t('Entity Table'),
    'help' => t('Entity Table'),
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

  // Entity ID.

  $data['civicrm_financial_item']['entity_id'] = array(
    'title' => t('Entity ID'),
    'help' => t('The numeric ID of the Entity'),
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

  /**
   * CiviCRM Financial Account Table
   */

  $data['civicrm_financial_account']['table']['group'] = t('CiviCRM Financial Account');

  $data['civicrm_financial_account']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Financial Account'),
    'help'  => t('View displays CiviCRM Financial Accounts.'),
  );

  // Internal Account ID.

  $data['civicrm_financial_account']['id'] = array (
    'title' => t('Financial Account ID'),
    'help'  => t('The unique ID of the Account in CiviCRM'),
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

  // Account Name.

  $data['civicrm_financial_account']['name'] = array (
    'title' => t('Name'),
    'help'  => t('The human readable name of the Account'),
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

  // Contact ID.

  $data['civicrm_financial_account']['contact_id'] = array (
    'title' => t('Contact ID'),
    'help'  => t('ID of the Contact who created the Financial Account'),
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

  // Account Type.

  $data['civicrm_financial_account']['financial_account_type_id'] = array (
    'title' => t('Financial Account Type'),
    'help'  => t('The Type of the Financial Account'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'accountOptionValues',
      'pseudo args' => array('financial_account_type'),
    ),
    'argument' => array(
      'handler' => 'civicrm_handler_argument_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'accountOptionValues',
      'pseudo args' => array('financial_account_type'),
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'accountOptionValues',
      'pseudo args' => array('financial_account_type'),
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // External Accounting Code.

  $data['civicrm_financial_account']['accounting_code'] = array (
    'title' => t('Accounting Code'),
    'help'  => t('The Accounting Code used for the Account in foreign systems'),
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

  // Account Type Code.

  $data['civicrm_financial_account']['account_type_code'] = array (
    'title' => t('Acount Type Code'),
    'help'  => t(''),
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

  // Description.

  $data['civicrm_financial_account']['description'] = array (
    'title' => t('Description'),
    'help'  => t('The Description of the Account'),
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

  // Parent Account ID.

  $data['civicrm_financial_account']['parent_id'] = array (
    'title' => t('Parent ID'),
    'help'  => t('The ID of the Parent account'),
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

  // Header Account field.

  $data['civicrm_financial_account']['is_header_account'] = array (
    'title' => t('Header Account'),
    'help'  => t(''),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Deductible account.

  $data['civicrm_financial_account']['is_deductible'] = array (
    'title' => t('Deductible'),
    'help'  => t('Is this account deductible?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Tax Account.

  $data['civicrm_financial_account']['is_tax'] = array (
    'title' => t('Is Tax'),
    'help'  => t('Is this a Tax Account?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Tax Rate.

  $data['civicrm_financial_account']['tax_rate'] = array (
    'title' => t('Tax Rate'),
    'help'  => t('The Tax rate this Account applies'),
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

  // Reserved Account.

  $data['civicrm_financial_account']['is_reserved'] = array (
    'title' => t('Reserved'),
    'help'  => t('Is this Account a reserved system account'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Enabled / Active.

  $data['civicrm_financial_account']['is_active'] = array (
    'title' => t('Active'),
    'help'  => t('Is this Account enabled?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Default Account.

  $data['civicrm_financial_account']['is_default'] = array (
    'title' => t('Default'),
    'help'  => t('Is this Account the default where it is used?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_boolean',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

}
