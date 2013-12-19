<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.4                                                |
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
 * Build the $data array for all the filed that are part of any CivicRM install
 * Includes the following tables
 * civicrm_contact
 * civicrm_address
 * civicrm_email
 * civicrm_website
 * civicrm_phone
 * civicrm_group
 * civicrm_tag
 * civicrm_relationship
 * civicrm_activity
 * civicrm_uf_match
 * All Custom fields
 */
function _civicrm_core_data(&$data, $enabled) {
  //------------------------------------------------------
  // CiviCRM Contacts Views Table
  //------------------------------------------------------

  $data['civicrm_contact']['table']['group'] = t('CiviCRM Contacts');

  $data['civicrm_contact']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Contacts'),
    'help' => t("View displays CiviCRM Contacts, of people, organizations, etc."),
  );

  //TABLE JOINS FOR CIVICRM CONTACTS GO HERE!

  $data['civicrm_contact']['table']['join'] = array(
    // Directly links to participants.
    'civicrm_participant' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    // Directly links to contribution
    'civicrm_contribution' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    // Directly links to activity.
    'civicrm_activity' => array(
      'left_field' => 'source_contact_id',
      'field' => 'id',
    ),
    // Directly links to membership.
    'civicrm_membership' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    // Link to mailing.
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    // Link to grant.
    'civicrm_grant' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
    // makes contact fields available to PCP views.
    'civicrm_pcp' => array(
      'left_field' => 'contact_id',
      'field' => 'id',
    ),
  );

  // For other base tables, explain how we join -

  $data['civicrm_contact']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'id',
  );
  $data['civicrm_uf_match']['table']['join']['users'] = array(
    'left_field' => 'uid',
    'field' => 'uf_id',
  );

  //CiviCRM Contacts - FIELDS


  //Numeric Contact ID
  $data['civicrm_contact']['id'] = array(
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
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Drupal ID of the contact (from uf_match.uf_id)
  $data['civicrm_contact']['drupal_id'] = array(
    'real field' => 'id',
    'title' => t('Drupal ID'),
    'help' => t('Relates a CiviCRM Contact to the Druapl User Record'),
    'field' => array(
      'handler' => 'civicrm_handler_field_drupalid',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'base' => 'users',
      'field' => 'uid',
      'handler' => 'civicrm_handler_relationship_contact2users',
      'label' => t('Drupal User'),
    ),
  );

  //SORT Name for the Contact (Last Name followed by First)
  $data['civicrm_contact']['sort_name'] = array(
    'title' => t('Sort Name'),
    'help' => t('The Contact Name for sorting purposes, Last Name then First'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contact_link',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //DISPLAY Name for the Contact (Full Name with Prefixes and Suffixes)
  $data['civicrm_contact']['display_name'] = array(
    'title' => t('Display Name'),
    'help' => t('Full Name of the Contact with prefixes and suffixes'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contact_link',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Gender
  $data['civicrm_contact']['gender'] = array(
    'title' => t('Gender'),
    'real field' => 'gender_id',
    'help' => t('Contact\'s Gender'),
    'field' => array(
      'handler' => 'civicrm_handler_field_gender',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_gender',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //FIRST NAME
  $data['civicrm_contact']['first_name'] = array(
    'title' => t('First Name'),
    'help' => t('First Name'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contact_link',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //MIDDLE NAME
  $data['civicrm_contact']['middle_name'] = array(
    'title' => t('Middle Name'),
    'help' => t('Middle Name'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contact_link',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //LAST NAME
  $data['civicrm_contact']['last_name'] = array(
    'title' => t('Last Name'),
    'help' => t('Last Name'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contact_link',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //BOOLEAN : IS Deceased
  $data['civicrm_contact']['is_deceased'] = array(
    'title' => t('Is Deceased'),
    'help' => t('Is the contact deceased?'),
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

  //THUMBSUP: Contact Permissions ie Do not email, do not this, do not that.

  //BOOLEAN : DO NOT EMAIL
  $data['civicrm_contact']['do_not_email'] = array(
    'title' => t('Do Not E-mail'),
    'help' => t('Does this contact not want to be emailed?'),
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

  //BOOLEAN : DO NOT PHONE
  $data['civicrm_contact']['do_not_phone'] = array(
    'title' => t('Do Not Phone'),
    'help' => t('Does this contact not want to be phoned?'),
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

  //BOOLEAN : DO NOT MAIL
  $data['civicrm_contact']['do_not_mail'] = array(
    'title' => t('Do Not Mail'),
    'help' => t('Does this contact not want to be mailed?'),
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

  //BOOLEAN : DO NOT SMS
  $data['civicrm_contact']['do_not_sms'] = array(
    'title' => t('Do Not Sms'),
    'help' => t('Does this contact not want to receive messages?'),
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

  //BOOLEAN : DO NOT TRADE
  $data['civicrm_contact']['do_not_trade'] = array(
    'title' => t('Do Not Trade'),
    'help' => t('Does this contact not want trade?'),
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

  //BOOLEAN : Opted out of Bulk Mailing
  $data['civicrm_contact']['is_opt_out'] = array(
    'title' => t('Opted Out of Bulk Mail'),
    'help' => t('Has this contact opted out of bulk mail?'),
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

  //BOOLEAN : Is Deleted
  $data['civicrm_contact']['is_deleted'] = array(
    'title' => t('Is Deleted'),
    'help' => t('Has this contact been deleted?'),
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

  // Organization Name
  $data['civicrm_contact']['current_employer'] = array(
    // The item it appears as on the UI,
    'title' => t('Organization Name'),
    'real field' => 'organization_name',
    // The help that appears on the UI,
    'help' => t('The display name of the contact\'s organizations.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // contact_type
  $data['civicrm_contact']['contact_type'] = array(
    // The item it appears as on the UI,
    'title' => t('Contact Type'),
    // The help that appears on the UI,
    'help' => t('Contact Type'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contact_type',
    ),
  );
  //Contact Sub Type
  $data['civicrm_contact']['contact_sub_type'] = array(
    'title' => t('Contact Sub Type'),
    'help' => t('The Sub-type for the Contact'),
    'field' => array(
      'handler' => 'civicrm_handler_field',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contact_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Birth Date
  $data['civicrm_contact']['birth_date'] = array(
    'title' => t('Birth Date'),
    'help' => t('The contact\'s birth date.'),
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
}

