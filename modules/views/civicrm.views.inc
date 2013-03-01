<?php
// $Id$

/*
 +--------------------------------------------------------------------+
 | CiviCRM version 3.4                                                |
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

/*
   * Copyright (C) 2009 James Rakich
   * Licensed to CiviCRM under the Academic Free License version 3.0.
   *
   */


/*
   *  civicrm.views.inc  Called from civicrm.module, gives the views cache all information it needs
   *                     to access CiviCRM's database for use in Views, as well as referencing the
   *                     custom handlers for displaying, sorting, filtering and accepting arguments
   *                     for this data.
   *
   // Defines tables, joins and relationships
   *  function civicrm_views_data() {
   *     CiviCRM Contacts Base Table
   *     CiviCRM Events Base Table
   *     CiviCRM Participants Base Table
   *     CiviCRM Contributions Base Table
   *     CiviCRM Activities Base Table
   *     CiviCRM Relationships Base Table
   *     CiviCRM Memberships Base Table
   *     CiviCRM Personal Campaign Pages Base Table
   *     CiviCRM Mailing Base Table
   *     CiviCRM Campaign Base Table
   *     CiviCRM Grants Base Table
   *     Text Links for each Base Table
   *     Custom Data Cache Query and Calls
   *
   *  function civicrm_views_href( $text, $path, $query )
   *     Generates a link for CiviCRM Paths - unchanged from previous code (anonymous donation)
   *
   *  function civicrm_views_custom_data_cache($data, $entity_type, $groupID, $subType, $style)
   *     Collects the data from Custom Data Groups and assigns them to base tables.
   *
   *  function civicrm_get_field_handler ($type)
   *  function civicrm_get_argument_handler ($type)
   *  function civicrm_get_filter_handler ($type)
   *  function civicrm_get_sort_handler ($type)
   *     Assign handlers to custom fields based on the data type (from the database records)
   *
   *  function civicrm_views_handlers ()
   *     Tells Views where to get the custom Handlers
   *
   *  function civicrm_date_api_tables()
   *  function civicrm_date_api_fields()
   *     Tells Views which fields to expose as Date API fields
   */

/**
 * Implementation of hook_views_data()
 */
function civicrm_views_data() {

  if (!civicrm_initialize()) {
    return;
  }

  require_once 'CRM/Core/Config.php';
  require_once 'CRM/Core/BAO/CustomGroup.php';
  require_once 'CRM/Core/DAO.php';
  require_once 'CRM/Core/Error.php';
  require_once 'CRM/Contact/BAO/Contact.php';
  require_once 'CRM/Event/BAO/Query.php';
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

  // add the other generic fields
  $objType = 'Individual';
  $fields = CRM_Contact_BAO_Contact::exportableFields($objType);

  $skipFields = array('id' => 1,
    'sort_name' => 1,
    'display_name' => 1,
    'gender' => 1,
    'first_name' => 1,
    'middle_name' => 1,
    'last_name' => 1,
    'is_deceased' => 1,
    'do_not_email' => 1,
    'do_not_phone' => 1,
    'do_not_mail' => 1,
    'do_not_trade' => 1,
    'is_opt_out' => 1,
    'contact_type' => 1,
    'current_employer' => 1,
    'birth_date' => 1,
  );
  civicrm_views_add_fields($fields, $data['civicrm_contact'], $skipFields, 'civicrm_contact');


  // CHILLING OUT AT THE CIVICRM_ADDRESS TABLE

  $data['civicrm_address']['table']['group'] = t('CiviCRM Address');

  // Explain how this table joins to others.
  $data['civicrm_address']['table']['join'] = array(
    // Directly links to contact table.
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );

  //CiviCRM Address Using Contacts as a pitstop before going on to Participants
  $data['civicrm_address']['table']['join']['civicrm_participant'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Address Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_address']['table']['join']['civicrm_contribution'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Address Using Contacts as a pitstop before going on to Activities
  $data['civicrm_address']['table']['join']['civicrm_activity'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Address Using Contacts as a pitstop before going on to PCP (makes address fields available to PCP views)
  $data['civicrm_address']['table']['join']['civicrm_pcp'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Address Using Locations as a pitstop before going on to Events
  $data['civicrm_address']['table']['join']['civicrm_event'] = array(
    'left_table' => 'civicrm_loc_block',
    'left_field' => 'address_id',
    'field' => 'id',
  );
  //CiviCRM Address Using Contacts as a pitstop before going on to Memberships
  $data['civicrm_address']['table']['join']['civicrm_membership'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  $data['civicrm_address']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );


  //BOOLEAN : IS PRIMARY ADDRESS
  $data['civicrm_address']['is_primary'] = array(
    'title' => t('Is Primary Address'),
    'help' => t('Is this address the contact\'s primary address?'),
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

  //BOOLEAN : IS BILLING ADDRESS
  $data['civicrm_address']['is_billing'] = array(
    'title' => t('Is Billing Address'),
    'help' => t('Is this address the contact\'s billing address?'),
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

  //ADDRESS'S LOCATION TYPE
  $data['civicrm_address']['location_type'] = array(
    'title' => t('Location Type'),
    'real field' => 'location_type_id',
    'help' => t('The Location of the Address for the Contact'),
    'field' => array(
      'handler' => 'civicrm_handler_field_location_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_location_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //FULL STREET ADDRESS
  $data['civicrm_address']['street_address'] = array(
    'title' => t('Full Street Address'),
    'help' => t('Full Street Address without Country, State or Postcode'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //FULL STREET ADDRESS SUPPLEMENTAL 1
  $data['civicrm_address']['supplemental_address_1'] = array(
    'title' => t('Supplemental Street Address'),
    'help' => t('Supplemental Street Address such as Apartment or Suite Number or Department'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //FULL STREET ADDRESS SUPPLEMENTAL 2
  $data['civicrm_address']['supplemental_address_2'] = array(
    'title' => t('2nd Supplemental Street Address'),
    'help' => t('2nd Supplemental Street Address such as Apartment or Suite Number or Department'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //CITY OR SUBURB THE ADDRESS IS AT
  $data['civicrm_address']['city'] = array(
    'title' => t('City / Suburb'),
    'help' => t('The City or Suburb the address is at'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //STATE OR PROVINCE
  $data['civicrm_address']['state_province'] = array(
    'title' => t('State / Province'),
    'real field' => 'state_province_id',
    'help' => t('The State or Province of the Address'),
    'field' => array(
      'handler' => 'civicrm_handler_field_state',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_state',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //ZIP / POSTAL CODE
  $data['civicrm_address']['postal_code'] = array(
    'title' => t('ZIP / Postal Code'),
    'help' => t('ZIP / Postal Code of the Address'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Latitude or Geo Code 1
  $data['civicrm_address']['geo_code_1'] = array(
    'title' => t('Latitude'),
    'help' => t('Latitude of the Address for mapping'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Longitude or Geo Code 2
  $data['civicrm_address']['geo_code_2'] = array(
    'title' => t('Longitude'),
    'help' => t('Longitude of the Address for mapping'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //COUNTRY
  $data['civicrm_address']['country'] = array(
    'title' => t('Country'),
    'real field' => 'country_id',
    'help' => t('The Country of the Address'),
    'field' => array(
      'handler' => 'civicrm_handler_field_country',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_country',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //COUNTY
  $data['civicrm_address']['county'] = array(
    'title' => t('County'),
    'real field' => 'county_id',
    'help' => t('The County of the Address'),
    'field' => array(
      'handler' => 'civicrm_handler_field_county',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_county',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Address name
  $link = l(t('In Global Settings'), 'civicrm/admin/setting/preferences/address', array('query' => 'reset=1'));
  $data['civicrm_address']['name'] = array(
    'title' => t('Address Name'),
    'help' => t('Be sure addres name is enabled %link', array('%link' => $link)),
    'field' => array(
      'handler' => 'views_handler_field',
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

  // CHILLING OUT AT THE CIVICRM_EMAIL TABLE

  $data['civicrm_email']['table']['group'] = t('CiviCRM Email');

  // Explain how this table joins to others.
  $data['civicrm_email']['table']['join'] = array(
    // Directly links to contact table.
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );

  //CiviCRM Email Using Contacts as a pitstop before going on to Participants
  $data['civicrm_email']['table']['join']['civicrm_participant'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Email Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_email']['table']['join']['civicrm_contribution'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Email Using Contacts as a pitstop before going on to Activities
  $data['civicrm_email']['table']['join']['civicrm_activity'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Email Using Locations as a pitstop before going on to Events
  $data['civicrm_email']['table']['join']['civicrm_event'] = array(
    'left_table' => 'civicrm_loc_block',
    'left_field' => 'email_id',
    'field' => 'id',
  );
  //CiviCRM Email Using Contacts as a pitstop before going on to Memberhsips
  $data['civicrm_email']['table']['join']['civicrm_membership'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //Display E-mails in User Views
  $data['civicrm_email']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );

  //BOOLEAN : IS PRIMARY EMAIL
  $data['civicrm_email']['is_primary'] = array(
    'title' => t('Is Primary Email'),
    'help' => t('Is this address the contact\'s primary email?'),
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

  //BOOLEAN : IS BILLING EMAIL
  $data['civicrm_email']['is_billing'] = array(
    'title' => t('Is Billing Email'),
    'help' => t('Is this address the contact\'s billing email?'),
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

  //EMAIL'S LOCATION TYPE
  $data['civicrm_email']['location_type'] = array(
    'title' => t('Location Type'),
    'real field' => 'location_type_id',
    'help' => t('The Location of the Address for the Contact'),
    'field' => array(
      'handler' => 'civicrm_handler_field_location_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_location_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //FULL EMAIL ADDRESS
  $data['civicrm_email']['email'] = array(
    'title' => t('Email Address'),
    'help' => t('Email Address'),
    'field' => array(
      'handler' => 'civicrm_handler_field_email',
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


  // CHILLING OUT AT THE CIVICRM_PHONE TABLE

  $data['civicrm_phone']['table']['group'] = t('CiviCRM Phone Details');

  // Explain how this table joins to others.
  $data['civicrm_phone']['table']['join'] = array(
    // Directly links to contact table.
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );

  //CiviCRM Phone Using Contacts as a pitstop before going on to Participants
  $data['civicrm_phone']['table']['join']['civicrm_participant'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Phone Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_phone']['table']['join']['civicrm_contribution'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Phone Using Contacts as a pitstop before going on to Activities
  $data['civicrm_phone']['table']['join']['civicrm_activity'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );


  //CiviCRM Phone Using Locations as a pitstop before going on to Events
  $data['civicrm_phone']['table']['join']['civicrm_event'] = array(
    'left_table' => 'civicrm_loc_block',
    'left_field' => 'phone_id',
    'field' => 'id',
  );
  //CiviCRM Phone Using Contacts as a pitstop before going on to Memberhsips
  $data['civicrm_phone']['table']['join']['civicrm_membership'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //Display Phone Details in User Views
  $data['civicrm_phone']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );


  //BOOLEAN : IS PRIMARY EMAIL
  $data['civicrm_phone']['is_primary'] = array(
    'title' => t('Is Primary Phone'),
    'help' => t('Is this the contact\'s primary phone?'),
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

  //BOOLEAN : IS BILLING EMAIL
  $data['civicrm_phone']['is_billing'] = array(
    'title' => t('Is Billing Phone'),
    'help' => t('Is this the contact\'s billing phone?'),
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

  //PHONE'S LOCATION TYPE
  $data['civicrm_phone']['location_type'] = array(
    'title' => t('Location Type'),
    'real field' => 'location_type_id',
    'help' => t('The Location of the Phone for the Contact'),
    'field' => array(
      'handler' => 'civicrm_handler_field_location_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_location_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //FULL PHONE NUMBER
  $data['civicrm_phone']['phone'] = array(
    'title' => t('Phone Number'),
    'help' => t('Full Phone Number'),
    'field' => array(
      'handler' => 'civicrm_handler_field_phone',
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

  //CONTACT ID contact this number belongs to
  $data['civicrm_phone']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('Contact phone number belongs to '),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //CONTACT ID contact this number belongs to
  $data['civicrm_phone']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('Contact phone number belongs to '),
    'field' => array(
      'handler' => 'views_handler_field',
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
    'relationship' => array(
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Related Phone'),
    ),
  );

  //TODO: Mobile Provider

  //PHONE TYPE  , Phone, Mobile, Fax or Pager
  $data['civicrm_phone']['phone_type'] = array(
    'title' => t('Phone Type'),
    'real field' => 'phone_type_id',
    'help' => t('The Type, either Phone, Mobile, Fax or Pager'),
    'field' => array(
      'handler' => 'civicrm_handler_field_phone_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_phone_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );


  // CHILLING OUT AT THE CIVICRM_WEBSITE TABLE

  $data['civicrm_website']['table']['group'] = t('CiviCRM Website');

  // Explain how this table joins to others.
  $data['civicrm_website']['table']['join'] = array(
    // Directly links to contact table.
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );

  //CiviCRM Website Using Contacts as a pitstop before going on to Participants
  $data['civicrm_website']['table']['join']['civicrm_participant'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Website Using Contacts as a pitstop before going on to Contributions
  $data['civicrm_website']['table']['join']['civicrm_contribution'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  //CiviCRM Website Using Contacts as a pitstop before going on to Activities
  $data['civicrm_website']['table']['join']['civicrm_activity'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );


  //CiviCRM Website Using Contacts as a pitstop before going on to Memberhsips
  $data['civicrm_website']['table']['join']['civicrm_membership'] = array(
    'left_table' => 'civicrm_contact',
    'left_field' => 'id',
    'field' => 'contact_id',
  );


  //Display Websites in User Views
  $data['civicrm_website']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );

  //WEBSITE TYPE
  $data['civicrm_website']['website_type'] = array(
    'title' => t('Website Type'),
    'real field' => 'website_type_id',
    'help' => t('The type of website for the contact'),
    'field' => array(
      'handler' => 'civicrm_handler_field_website_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_website_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //FULL WEB ADDRESS
  $data['civicrm_website']['url'] = array(
    'title' => t('Website URL'),
    'help' => t('Website URL'),
    'field' => array(
      'handler' => 'views_handler_field',
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
    'relationship' => array(
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Related Phone'),
    ),
  );




  //----------------------------------------------------------------
  // CIVICRM Events Be next bro! It's A BASE TABLE
  //----------------------------------------------------------------

  $data['civicrm_event']['table']['group'] = t('CiviCRM Events');

  $data['civicrm_event']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Events'),
    'help' => t("View displays CiviCRM Events"),
  );

  //TABLE JOINS FOR CIVICRM EVENTS GO HERE!

  $data['civicrm_event']['table']['join'] = array(
    // Directly links to contact table.
    'civicrm_participant' => array(
      'left_field' => 'event_id',
      'field' => 'id',
    ),
  );

  // Numeric Event ID
  $data['civicrm_event']['id'] = array(
    'title' => t('Event ID'),
    'help' => t('The numeric ID of the Event'),
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

  //Event Type
  $data['civicrm_event']['event_type'] = array(
    'title' => t('Event Type'),
    'real field' => 'event_type_id',
    'help' => t('The Event Type'),
    'field' => array(
      'handler' => 'civicrm_handler_field_event_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_event_type',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //BOOLEAN : IS EVENT PUBLIC
  $data['civicrm_event']['is_public'] = array(
    'title' => t('Is Public'),
    'help' => t('Has this event been made public?'),
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

  //BOOLEAN : IS EVENT ACTIVE
  $data['civicrm_event']['is_active'] = array(
    'title' => t('Is Active'),
    'help' => t('Has this event been made active?'),
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
  //BOOLEAN : Include EVENT Templates ?
  $data['civicrm_event']['is_template'] = array(
    'title' => t('Include Template'),
    'help' => t('Include Event templates?'),
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
  //EVENT TITLE
  $data['civicrm_event']['title'] = array(
    'title' => t('Title'),
    'help' => t('The Event\'s Title'),
    'field' => array(
      'handler' => 'civicrm_handler_field_event_link',
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

  // EVENT SUMMARY
  $data['civicrm_event']['summary'] = array(
    'title' => t('Summary'),
    'help' => t('The Event\'s Summary'),
    'field' => array(
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => TRUE,
    ),
  );

  // EVENT DESCRIPTION
  $data['civicrm_event']['description'] = array(
    'title' => t('Description'),
    'help' => t('The Event\'s Full Description'),
    'field' => array(
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => TRUE,
    ),
  );

  //EVENT Start DATE
  $data['civicrm_event']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The Event\'s Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_event'], array('title' => 'Start Date',
      'name' => 'start_date',
    ));
  //EVENT END DATE
  $data['civicrm_event']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The Event\'s End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_event'], array('title' => 'End Date',
      'name' => 'end_date',
    ));
  //BOOLEAN : IS ONLINE REGISTRATION ENABLED
  $data['civicrm_event']['is_online_registration'] = array(
    'title' => t('Is Online Registration On'),
    'help' => t('Is Online Registration enabled for the event?'),
    'field' => array(
      'handler' => 'views_handler_field_boolean',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_boolean_operator',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //EVENT ONLINE REGISTRATION START DATE
  $data['civicrm_event']['registration_start_date'] = array(
    'title' => t('Registration Start Date'),
    'help' => t('The Event\'s Online Registration Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_event'], array('title' => 'Registration Start Date',
      'name' => 'registration_start_date',
    ));
  //EVENT ONLINE REGISTRATION END DATE
  $data['civicrm_event']['registration_end_date'] = array(
    'title' => t('Registration End Date'),
    'help' => t('The Event\'s Online Registration End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_event'], array('title' => 'Registration End Date',
      'name' => 'registration_end_date',
    ));

  //EVENT LINK TEXT
  $data['civicrm_event']['registration_link_text'] = array(
    'title' => t('Registration Link Text'),
    'help' => t('Text for the Registration Link'),
    'field' => array(
      'handler' => 'civicrm_handler_field_event_link',
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
  // Link to Campaign Table
  $data['civicrm_event']['campaign_id'] = array(
    'title' => t('Campaign'),
    'help' => t('Event -> Campaign'),
    'relationship' => array(
      'base' => 'civicrm_campaign',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Event -> Campaign'),
    ),
  );

  // CHILLING OUT AT THE CIVICRM_LOC_BLOCK TABLE

  $data['civicrm_loc_block']['table']['group'] = t('CiviCRM Locations');

  // Explain how this table joins to others.
  $data['civicrm_loc_block']['table']['join'] = array(
    // Directly links to event table
    'civicrm_event' => array(
      'left_field' => 'loc_block_id',
      'field' => 'id',
    ),
  );


  //----------------------------------------------------------------
  // CIVICRM Participants are here, base tabling it up.
  //----------------------------------------------------------------

  $data['civicrm_participant']['table']['group'] = t('CiviCRM Participants');

  $data['civicrm_participant']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Participants'),
    'help' => t("View displays CiviCRM Participants, often in conjunction with Event Filters"),
  );

  //TABLE JOINS FOR CIVICRM PARTICIPANTS GO HERE!
  // display user fields in Participant view
  $data['civicrm_participant']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );
  $data['users']['table']['join']['civicrm_participant'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'uf_id',
    'field' => 'uid',
  );
  //Display Participants in User Views
  $data['civicrm_uf_match']['table']['join']['civicrm_participant'] = array(
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );


  //CiviCRM Participants - FIELDS

  //Numeric Participant ID
  $data['civicrm_participant']['id'] = array(
    'title' => t('Participant ID'),
    'help' => t('The numeric ID of the Participant'),
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
  //Numeric Event ID
  $data['civicrm_participant']['event_id'] = array(
    'title' => t('Event ID'),
    'help' => t('The numeric ID of the Event'),
    'relationship' => array(
      'base' => 'civicrm_event',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Event, with custom fields'),
    ),
  );

  //Numeric Contact ID
  $data['civicrm_participant']['contact_id'] = array(
    'title' => t('Participant\'s Contact ID'),
    'help' => t('The numeric ID of the Contact of the Participant'),
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
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contact, with custom fields'),
    ),
  );

  //Participant Status
  $data['civicrm_participant']['status'] = array(
    'title' => t('Participant Status'),
    'real field' => 'status_id',
    'help' => t('The Status of the Participant\'s Booking and Attendance'),
    'field' => array(
      'handler' => 'civicrm_handler_field_participant_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_participant_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Participant Role
  $data['civicrm_participant']['role'] = array(
    'title' => t('Participant Role'),
    'real field' => 'role_id',
    'help' => t('The Role the Participant is registered for.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_participant_role',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_participant_role',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Source
  $data['civicrm_participant']['source'] = array(
    'title' => t('Source'),
    'help' => t('Where did this registration come from?'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Participant Registration Date
  $data['civicrm_participant']['register_date'] = array(
    'title' => t('Register Date'),
    'help' => t('The Date the participant Registered for the Event'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_participant'], array('title' => 'Register Date',
      'name' => 'register_date',
    ));

  //BOOLEAN : IS SET TO PAY LATER
  $data['civicrm_participant']['is_pay_later'] = array(
    'title' => t('Is Pay Later'),
    'help' => t('Opted to Pay Later after Registration, and is still pending payment.'),
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

  // Registered by ID - Participant ID of the person who made the registration
  $data['civicrm_participant']['registered_by_id'] = array(
    'title' => t('Registered by ID'),
    'help' => t('The numeric ID of the Participant who registered this Participant'),
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
    'relationship' => array(
      'base' => 'civicrm_participant',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Source Record Contact'),
    ),
  );

  //Fee Level
  $data['civicrm_participant']['fee_level'] = array(
    'title' => t('Fee Level'),
    'help' => t('The Fee Level the Participant is registered for.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  // Fee Amount , the amount the Participant has paid or will have to pay.
  $data['civicrm_participant']['fee_amount'] = array(
    'title' => t('Fee Amount'),
    'help' => t('The amount the Participant has paid or will have to pay.'),
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
  // Link to Campaign Table
  $data['civicrm_participant']['campaign_id'] = array(
    'title' => t('Campaign'),
    'help' => t('Participant -> Campaign'),
    'relationship' => array(
      'base' => 'civicrm_campaign',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Participant -> Campaign'),
    ),
  );

  // HOP SCOTCHING OUR WAY THROUGH THE CIVICRM_PARTICIPANT_PAYMENT

  // Explain how this table joins to others.
  $data['civicrm_participant_payment']['table']['join'] = array(
    // Directly links to event table
    'civicrm_participant' => array(
      'left_field' => 'id',
      'field' => 'participant_id',
    ),
  );




  //----------------------------------------------------------------
  // CIVICRM Contributions are here with all the bling, base tabling it up.
  //----------------------------------------------------------------

  $data['civicrm_contribution']['table']['group'] = t('CiviCRM Contributions');

  $data['civicrm_contribution']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Contributions'),
    'help' => t("View displays CiviCRM Contributions, with relation to contacts."),
  );

  //CiviCRM recurring contributions - make this available as a base table too
  $data['civicrm_contribution_recur']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Recurring Contribution profile'),
    'help' => t("Recurring Contribution Profiles in CiviCRM."),
  );
  //this join makes Contribution data available in view based on recurring . A separate relationship is defined elsewhere to make recurring data available to view based on contributi
  $data['civicrm_contribution']['table']['join']['civicrm_contribution_recur'] = array(
    'left_field' => 'id',
    'field' => 'contribution_recur_id',
  );
  //TABLE JOINS FOR CIVICRM CONTRIBUTIONS GO HERE!

  $data['civicrm_contribution']['table']['join']['civicrm_participant'] = array(
    'left_table' => 'civicrm_participant_payment',
    'left_field' => 'contribution_id',
    'field' => 'id',
  );
  $data['civicrm_contribution']['table']['join']['civicrm_pledge'] = array(
    'left_table' => 'civicrm_pledge_payment',
    'left_field' => 'contribution_id',
    'field' => 'id',
  );
  //CiviCRM Contributions - FIELDS

  //Numeric Contribution ID
  $data['civicrm_contribution']['id'] = array(
    'title' => t('Contribution ID'),
    'help' => t('The numeric ID of the Contribution'),
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
  //Contact ID for relationship to contact record (brings in Contact custom fields)
  $data['civicrm_contribution']['contact_id'] = array(
    'title' => t('Contact Record ID'),
    'help' => 'Bring Contact Record the complete contact record to the party',
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Contact, with custom fields'),
    ),
  );
  //Contribution Type
  $data['civicrm_contribution']['contribution_type'] = array(
    'title' => t('Contribution Type'),
    'real field' => 'contribution_type_id',
    'help' => t('The Type of Contribution made, ie Member Dues, Event Fees etc'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contribution_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contribution_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Contribution Page
  $data['civicrm_contribution']['contribution_page'] = array(
    'title' => t('Contribution Page'),
    'real field' => 'contribution_page_id',
    'help' => t('The Page this Contribution came from'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contribution_page',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contribution_page',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Payment Instrument Used
  $data['civicrm_contribution']['payment_instrument'] = array(
    'title' => t('Payment Instrument'),
    'real field' => 'payment_instrument_id',
    'help' => t('The Payment Instrument Used for the Contribution'),
    'field' => array(
      'handler' => 'civicrm_handler_field_payment_instrument',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_payment_instrument',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );



  //Contribution Receive Date
  $data['civicrm_contribution']['receive_date'] = array(
    'title' => t('Receive Date'),
    'help' => t('The Date the Contribution was received / paid'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_contribution'], array('title' => 'Receive Date',
      'name' => 'receive_date',
    ));
  //Non Deductible Amount of Contribution
  $data['civicrm_contribution']['non_deductible_amount'] = array(
    'title' => t('Non Deductible Amount'),
    'help' => t('The Non Deductible Part of the Contribution'),
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

  $data['civicrm_contribution']['total_amount'] = array(
    'title' => t('Total Amount'),
    'help' => t('The Total Contribution made by the Contact'),
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

  $data['civicrm_contribution']['fee_amount'] = array(
    'title' => t('Fee Amount'),
    'help' => t('The fee charged by the Payment Processor for the transaction.'),
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

  $data['civicrm_contribution']['net_amount'] = array(
    'title' => t('Net Amount'),
    'help' => t('The Full Contribution minus Payment Processor Fees.'),
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

  //Transaction ID
  $data['civicrm_contribution']['trxn_id'] = array(
    'title' => t('Transaction ID'),
    'help' => t('Unique Transaction ID from Payment Processor / Bank etc'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Invoice ID
  $data['civicrm_contribution']['invoice_id'] = array(
    'title' => t('Invoice ID'),
    'help' => t('Unique Invoice ID from system'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Currency
  $data['civicrm_contribution']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency used for the Contribution'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Contribution Cancellation Date
  $data['civicrm_contribution']['cancel_date'] = array(
    'title' => t('Cancellation Date'),
    'help' => t('The Date the Contribution was cancelled (if it was)'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution'], array('title' => 'Cancel Date',
      'name' => 'cancel_date',
    ));

  //Cancellation Reason
  $data['civicrm_contribution']['cancel_reason'] = array(
    'title' => t('Cancellation Reason'),
    'help' => t('The Reason for the Contribution Cancellation'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Contribution Receipt Date
  $data['civicrm_contribution']['receipt_date'] = array(
    'title' => t('Receipt Date'),
    'help' => t('The Date a receipt for the contribution was sent.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution'], array('title' => 'Receipt Date',
      'name' => 'receipt_date',
    ));

  //Contribution Thank You Date
  $data['civicrm_contribution']['thankyou_date'] = array(
    'title' => t('Thank You Date'),
    'help' => t('The Date a thank you to the donor for the contribution was sent.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution'], array('title' => 'Thank You Date',
      'name' => 'thankyou_date',
    ));

  //Contribution Source
  $data['civicrm_contribution']['source'] = array(
    'title' => t('Source'),
    'help' => t('Where the Contribution came through.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Contribution Status
  $data['civicrm_contribution']['contribution_status'] = array(
    'title' => t('Contribution Status'),
    'real field' => 'contribution_status_id',
    'help' => t('The Status of this Contribution'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contribution_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contribution_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //TODO: CONTRIBUTION RECURRING STUFF + HONOR_CONTACT_ID + HONOR_TYPE_ID

  //BOOLEAN : IS A TEST CONTRIBUTION
  $data['civicrm_contribution']['is_test'] = array(
    'title' => t('Is Test'),
    'help' => t('Is the contribution a test entry?'),
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

  //BOOLEAN : IS SET TO PAY LATER
  $data['civicrm_contribution']['is_pay_later'] = array(
    'title' => t('Is Pay Later'),
    'help' => t('Opted to Pay Contribution Later, and is still pending payment.'),
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
  $data['civicrm_contribution']['check_number'] = array(
    'title' => t('Check Number'),
    'help' => t('Contribution\'s Check Number'),
    'field' => array(
      'handler' => 'views_handler_field',
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


  $data['civicrm_contribution']['campaign_id'] = array(
    'title' => t('Campaign'),
    'help' => t('Contribution -> Campaign'),
    'relationship' => array(
      'base' => 'civicrm_campaign',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Contribution -> Campaign'),
    ),
  );


  /*
     * Soft Credits
     */


  $data['civicrm_contribution_soft']['table']['group'] = t('CiviCRM  Soft Credits');


  //TABLE JOINS FOR CIVICRM CONTRIBUTIONS GO HERE!
  $data['civicrm_contribution_soft']['table']['join']['civicrm_contribution'] = array(
    'left_table' => 'civicrm_contribution',
    'left_field' => 'id',
    'field' => 'contribution_id',
  );
  //CiviCRM Soft Credits - FIELDS

  //Numeric Soft Credit ID
  $data['civicrm_contribution_soft']['id'] = array(
    'title' => t('Contribution Soft Credit ID'),
    'help' => t('The numeric ID of the Soft Credit'),
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

  //Numeric Contact ID of the Soft Credit recipient
  $data['civicrm_contribution_soft']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('Contact ID of the Soft Credit recipient'),
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
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Soft Contribution -> Contact Record'),
    ),
  );
  //Soft Credit Amount
  $data['civicrm_contribution_soft']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The amount of the soft credit'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Personal Campaign Page
  $data['civicrm_contribution_soft']['pcp_roll_nickname'] = array(
    'title' => t('Personal Campaign Page Nickname'),
    'help' => t('The Page this Contribution came from'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //Personal Campaign Page Personla Note
  $data['civicrm_contribution_soft']['pcp_personal_note'] = array(
    'title' => t('Personal Campaign Page Personal Note'),
    'help' => t('The Personal note form PCP'),
    'field' => array(
      'handler' => 'views_handler_field',
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


  /*
     * Soft Credits
     */


  $data['civicrm_contribution_recur']['table']['group'] = t('CiviCRM  Recurring Contributions');


  //TABLE JOINS FOR CIVICRM CONTRIBUTIONS GO HERE!
  $data['civicrm_contribution_recur']['table']['join']['civicrm_contribution'] = array(
    'left_table' => 'civicrm_contribution',
    'left_field' => 'contribution_recur_id',
    'field' => 'id',
  );

  //CiviCRM Soft Credits - FIELDS

  //Numeric Recurring Contribution ID
  $data['civicrm_contribution_recur']['id'] = array(
    'title' => t('Recurring Contribution ID'),
    'help' => t('The numeric ID of the  Recurring Contribution'),
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

  $data['civicrm_contribution_recur']['contact_id'] = array(
    'title' => t('Contact ID'),
    'help' => t('The contact who is paying the recurring pledge'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Recurring payment Contact'),
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

  //Recurring Contribution Amount
  $data['civicrm_contribution_recur']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The amount of the recurring contribution'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //Frequency of recurring contribution
  $data['civicrm_contribution_recur']['frequency_unit'] = array(
    'title' => t('Frequency Unit'),
    'help' => t('Day, Week, Month, Year'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //Frequency Interval
  $data['civicrm_contribution_recur']['frequency_interval'] = array(
    'title' => t('Frequency Interval'),
    'help' => t('The Interval of the Frequency Unit'),
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
  //# of Installments
  $data['civicrm_contribution_recur']['installments'] = array(
    'title' => t('Installments'),
    'help' => t('Number of Installments'),
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
  //Recurring Contribution Start Date
  $data['civicrm_contribution_recur']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The Date the recurring contribution started.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution Start Date',
      'name' => 'start_date',
    ));
  //Recurring Contribution Create Date
  $data['civicrm_contribution_recur']['create_date'] = array(
    'title' => t('Create Date'),
    'help' => t('The Date the recurring contribution was created.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution Create Date',
      'name' => 'create_date',
    ));
  //Recurring Contribution Modified Date
  $data['civicrm_contribution_recur']['modified_date'] = array(
    'title' => t('Modified Date'),
    'help' => t('The Date the recurring contribution was modified.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution Modified Date',
      'name' => 'modified_date',
    ));
  //Recurring Contribution Cancel Date
  $data['civicrm_contribution_recur']['cancel_date'] = array(
    'title' => t('Cancel Date'),
    'help' => t('The Date the recurring contribution was cancelled.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution Cancel Date',
      'name' => 'cancel_date',
    ));
  //Recurring Contribution End Date
  $data['civicrm_contribution_recur']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The Date the recurring contribution ended.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution End Date',
      'name' => 'end_date',
    ));
  //Unique Transaction ID
  $data['civicrm_contribution_recur']['trxn_id'] = array(
    'title' => t('Unique Transaction ID'),
    'help' => t('The Transaction ID of the  Recurring Contribution'),
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
  //Invoice ID
  $data['civicrm_contribution_recur']['invoice_id'] = array(
    'title' => t('Invoice ID'),
    'help' => t('The Unique CiviCRM Generated Invoice ID'),
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
  //recurring contributions Status
  $data['civicrm_contribution_recur']['contiribution_status'] = array(
    'title' => t('Contribution Status'),
    'real field' => 'contribution_status_id',
    'help' => t('The Contribution\'s Status'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contribution_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contribution_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //BOOLEAN : IS A TEST CONTRIBUTION
  $data['civicrm_contribution_recur']['is_test'] = array(
    'title' => t('Is Test'),
    'help' => t('Is the recurring contribution a test entry?'),
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
  //Cycle contibution Day
  $data['civicrm_contribution_recur']['cycle_day'] = array(
    'title' => t('Cycle Day'),
    'help' => t('What day in the contribution will the debit occur'),
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
  //Recurring Contribution End Date
  $data['civicrm_contribution_recur']['next_sched_contribution'] = array(
    'title' => t('Next Contribution Date'),
    'help' => t('The Next Contribution Date.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution Next Date',
      'name' => 'next_sched_contribution',
    ));
  //How many times has the contribution failed
  $data['civicrm_contribution_recur']['failure_count'] = array(
    'title' => t('Failure Count'),
    'help' => t('How many times has theis recurring contribution failed?'),
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
  //when will the filed contribution be retried?
  $data['civicrm_contribution_recur']['failure_retry_date'] = array(
    'title' => t('Failed retry date'),
    'help' => t('When will the failed contribution be retried?'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_contribution_recur'], array('title' => 'Recurring Contribution Failure Retry Date',
      'name' => 'failure_retry_date',
    ));
  //BOOLEAN : Does thie recurring contribution auto-renew?
  $data['civicrm_contribution_recur']['auto_renew'] = array(
    'title' => t('Auto-renew'),
    'help' => t('Will this recurring contribution automayically renew?'),
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



  $data['civicrm_pledge']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Pledges'),
    'help' => t("CiviCRM Pledges."),
  );
  $data['civicrm_pledge']['table']['group'] = t('CiviCRM Pledges');
  $data['civicrm_pledge_payment']['table']['group'] = t('Pledge Payments');
  $data['civicrm_pledge']['id'] = array(
    'title' => t('CiviCRM Pledge ID'),
    'help' => t('The numeric ID of the Pledge'),
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
  $data['civicrm_pledge']['amount'] = array(
    'title' => t('Amount'),
    'help' => t('The total amount of the pledge'),
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
  //----------------------------------------------------------------------------------------
  // CIVICRM Activities are here with all the activeness they can muster, base tabling it up.
  //----------------------------------------------------------------------------------------

  $data['civicrm_activity']['table']['group'] = t('CiviCRM Activities');

  $data['civicrm_activity']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Activities'),
    'help' => t("View displays CiviCRM Activities"),
  );

  //TABLE JOINS FOR CIVICRM ACTIVITIES GO HERE!

  //CiviCRM Activities - FIELDS

  //Numeric Activity ID
  $data['civicrm_activity']['id'] = array(
    'title' => t('Activity ID'),
    'help' => t('The numeric ID of the Activity Instance'),
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

  $data['civicrm_activity']['source_contact_id'] = array(
    'title' => t('Source Contact'),
    'help' => t('The contact who scheduled this activity'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Source Contact'),
    ),
  );

  $data['civicrm_activity']['source_record_id'] = array(
    'title' => t('Source Record ID'),
    'help' => t('The contact who made this activity'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Source Record Contact'),
    ),
  );

  //Activity Type
  $data['civicrm_activity']['activity_type'] = array(
    'title' => t('Activity Type'),
    'real field' => 'activity_type_id',
    'help' => t('The Type of Activity'),
    'field' => array(
      'handler' => 'civicrm_handler_field_activity_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_activity_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Activity Status
  $data['civicrm_activity']['status'] = array(
    'title' => t('Activity Status'),
    'real field' => 'status_id',
    'help' => t('The Status of this Activity'),
    'field' => array(
      'handler' => 'civicrm_handler_field_activity_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_activity_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Activity Priority
  $data['civicrm_activity']['priority'] = array(
    'title' => t('Priority'),
    'real field' => 'priority_id',
    'help' => t('The Priority of this Activity'),
    'field' => array(
      'handler' => 'civicrm_handler_field_priority',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_priority',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //Parent ID
  $data['civicrm_activity']['parent_id'] = array(
    'title' => t('Parent Activity ID'),
    'help' => t('The numeric ID of the Parent Activity '),
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

  //Activity Subject
  $data['civicrm_activity']['subject'] = array(
    'title' => t('Subject'),
    'help' => t('A short description of the activity..'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Scheduled Activity Date
  $data['civicrm_activity']['activity_date_time'] = array(
    'title' => t('Scheduled Activity Date'),
    'help' => t('The Date and Time the activity is scheduled to happen.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  civicrm_views_add_date_arguments($data['civicrm_activity'], array('title' => 'Scheduled Activity Date',
      'name' => 'activity_date_time',
    ));

  //Activity Duration
  $data['civicrm_activity']['duration'] = array(
    'title' => t('Duration (min)'),
    'help' => t('The Activity\'s Duration in Minutes'),
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

  //Activity Location
  $data['civicrm_activity']['location'] = array(
    'title' => t('Location'),
    'help' => t('Activity\'s Location in a text string.'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Activity Details / Notes
  $data['civicrm_activity']['details'] = array(
    'title' => t('Details'),
    'help' => t('Details and Notes regarding the activity.'),
    'field' => array(
      'handler' => 'views_handler_field',
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


  //BOOLEAN : IS A TEST ACTIVITY
  $data['civicrm_activity']['is_test'] = array(
    'title' => t('Is Test'),
    'help' => t('Is the activity a test entry?'),
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
  //Activity Medium (ie Phone, e-mail etc...)
  $data['civicrm_activity']['medium'] = array(
    'title' => t('Medium'),
    'real field' => 'medium_id',
    'help' => t('The Medium of this Activity (ie phone, e-mail, etc...'),
    'field' => array(
      'handler' => 'civicrm_handler_field_encounter_medium',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_encounter_medium',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //BOOLEAN : IS A TEST ACTIVITY
  $data['civicrm_activity']['is_auto'] = array(
    'title' => t('Is Automaticaly created'),
    'help' => t('Is the activity created by the system?'),
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
  // Link to Campaign Table
  $data['civicrm_activity']['campaign_id'] = array(
    'title' => t('Campaign'),
    'help' => t('Activity -> Campaign'),
    'relationship' => array(
      'base' => 'civicrm_campaign',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Activity -> Campaign'),
    ),
  );
  $data['civicrm_activity']['engagement_level'] = array(
    'title' => t('Engagement level'),
    'help' => t('The Engagement Level of this Activity for this contact'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );



  // CHILLING OUT AT THE CIVICRM_ACTIVTY_ASSIGNMENT TABLE

  $data['civicrm_activity_assignment']['table']['group'] = t('CiviCRM Activity Assignments');

  // Explain how this table joins to others.
  $data['civicrm_activity_assignment']['table']['join'] = array(
    // Directly links to event table
    'civicrm_activity' => array(
      'left_field' => 'id',
      'field' => 'activity_id',
    ),
  );

  $data['civicrm_activity_assignment']['assignee_contact_id'] = array(
    'title' => t('Assignee Contact ID'),
    'help' => t('The contact who is assigned the activity.'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Assignee Contact ID'),
    ),
  );

  // CHILLING OUT AT THE CIVICRM_ACTIVTY_ASSIGNMENT TABLE

  $data['civicrm_activity_target']['table']['group'] = t('CiviCRM Activity Targets');

  // Explain how this table joins to others.
  $data['civicrm_activity_target']['table']['join'] = array(
    // Directly links to event table
    'civicrm_activity' => array(
      'left_field' => 'id',
      'field' => 'activity_id',
    ),
  );

  $data['civicrm_activity_target']['target_contact_id'] = array(
    'title' => t('Target Contact ID'),
    'help' => t('The contact who is the activity\'s target.'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('CiviCRM Target Contact ID'),
    ),
  );

  //TEXT LINKS THAT AREN'T PART OF THE TABLE
  //Contact Text Link
  $data['civicrm_contact']['text_link'] = array(
    'field' => array(
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various contact actions.'),
      'handler' => 'civicrm_handler_field_link_contact',
    ),
  );

  //Event Text Link
  $data['civicrm_event']['text_link'] = array(
    'field' => array(
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various event actions.'),
      'handler' => 'civicrm_handler_field_link_event',
    ),
  );



  //Participant Text Link
  $data['civicrm_participant']['text_link'] = array(
    'field' => array(
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various participant actions.'),
      'handler' => 'civicrm_handler_field_link_participant',
    ),
  );

  //Contribution Text Link
  $data['civicrm_contribution']['text_link'] = array(
    'field' => array(
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various contribution actions.'),
      'handler' => 'civicrm_handler_field_link_contribution',
    ),
  );


  //----------------------------------------------------------------
  // CIVICRM Relationships are here with all the connections, base tabling it up.
  //----------------------------------------------------------------


  $data['civicrm_relationship']['table']['group'] = t('CiviCRM Relationships');

  $data['civicrm_relationship']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Relationships'),
    'help' => t("View displays CiviCRM Relationships, with connection to contacts."),
  );

  //TABLE JOINS FOR CIVICRM RELATIONSHIPS GO HERE!

  $data['civicrm_relationship']['table']['join'] = array(
    // Directly links to contact table - link A
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id_a',
    ),
    // Directly links to contact table - link B also.
    //		   'civicrm_contact' => array(
    //									  'left_field' => 'id',
    //									  'field' => 'contact_id_b',
    //									  ),
  );

  //Display A->B relationships in User Views
  $data['civicrm_relationship']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id_a',
  );



  //CiviCRM Relationships - FIELDS

  //Numeric Relationship ID
  $data['civicrm_relationship']['id'] = array(
    'title' => t('Relationship ID'),
    'help' => t('The numeric ID of the Relationship'),
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

  //Relationship Type
  $data['civicrm_relationship']['relationship_type'] = array(
    'title' => t('Relationship Type A-to-B'),
    'real field' => 'relationship_type_id',
    'help' => t('The Type of Relationship, i.e. Employee of, Household Head for...'),
    'field' => array(
      'handler' => 'civicrm_handler_field_relationship_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_relationship_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['civicrm_relationship']['contact_id_a'] = array(
    'title' => t('Contact ID A'),
    'real field' => 'contact_id_a',
    'help' => t('The contact A'),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'relationship table' => 'civicrm_relationship',
      'relationship field' => 'contact_id_a',
      'other_field' => 'contact_id_b',
      'handler' => 'civicrm_handler_relationship',
      'label' => t('CiviCRM Contact A'),
    ),
  );

  $data['civicrm_relationship']['contact_id_b'] = array(
    'title' => t('Contact ID B'),
    'real field' => 'contact_id_b',
    'help' => t('The contact B'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'field' => 'id',
      'relationship table' => 'civicrm_relationship',
      'relationship field' => 'contact_id_b',
      'other_field' => 'contact_id_a',
      'handler' => 'civicrm_handler_relationship',
      'label' => t('CiviCRM Contact B'),
    ),
  );


  $data['civicrm_relationship']['description'] = array(
    'title' => t('Description'),
    'help' => t('Description of the relationship'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Relationship start date
  $data['civicrm_relationship']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The Relationship\'s Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_relationship'], array('title' => 'Start Date',
      'name' => 'start_date',
    ));

  //Relationship end date
  $data['civicrm_relationship']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The Relationship\'s End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_replationship'], array('title' => 'AEnd Date',
      'name' => 'end_date',
    ));
  //BOOLEAN : IS RELATIONSHIP ACTIVE
  $data['civicrm_relationship']['is_active'] = array(
    'title' => t('Is Relationship Active'),
    'help' => t('Is the Relationship active?'),
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

  //----------------------------------------------------------------
  // CIVICRM Memberships are here with all the connections.
  //----------------------------------------------------------------


  $data['civicrm_membership']['table']['group'] = t('CiviCRM Member');

  $data['civicrm_membership']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Memberships'),
    'help' => t("View displays CiviCRM Memberships, with connection to contacts."),
  );
  $data['civicrm_membership']['table']['join'] = array(
    // Directly links to contact table - link A
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );

  //Display Memberships in User Views
  $data['civicrm_membership']['table']['join']['users'] = array(
    'left_table' => 'civicrm_uf_match',
    'left_field' => 'contact_id',
    'field' => 'contact_id',
  );



  //CiviCRM Memberships - FIELDS

  //Numeric Membership ID
  $data['civicrm_membership']['id'] = array(
    'title' => t('Membership ID'),
    'help' => t('The numeric ID of the Membership'),
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

  //Membership Type
  $data['civicrm_membership']['membership_type'] = array(
    'title' => t('Membership Type'),
    'real field' => 'membership_type_id',
    'help' => t('The Type of Membership'),
    'field' => array(
      'handler' => 'civicrm_handler_field_membership_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_membership_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Membership Join date
  $data['civicrm_membership']['join_date'] = array(
    'title' => t('Join Date'),
    'help' => t('The Membership\'s Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_membership'], array('title' => 'Join Date',
      'name' => 'join_date',
    ));

  //Membership start date
  $data['civicrm_membership']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The Membership\'s Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_membership'], array('title' => 'Start Date',
      'name' => 'start_date',
    ));


  //Membership end date
  $data['civicrm_membership']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The Membership\'s End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_membership'], array('title' => 'End Date',
      'name' => 'end_date',
    ));


  //membership Source
  $data['civicrm_membership']['source'] = array(
    'title' => t('Source'),
    'help' => t('Source of the relationship'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //Membership Status
  $data['civicrm_membership']['status'] = array(
    'title' => t('Status'),
    'real field' => 'status_id',
    'help' => t('The Status of the Membership'),
    'field' => array(
      'handler' => 'civicrm_handler_field_membership_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_membership_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //BOOLEAN : IS Status Overridden
  $data['civicrm_membership']['is_override'] = array(
    'title' => t('Status overridden?'),
    'help' => t('Is the membership status overridden?'),
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
  //Membership end date
  $data['civicrm_membership']['reminder_date'] = array(
    'title' => t('Reminder Date'),
    'help' => t('The Membership\'s Reminder Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_membership'], array('title' => 'Reminder Date',
      'name' => 'reminder_date',
    ));



  //Numeric Membership Owner ID
  $data['civicrm_membership']['owner_membership_id'] = array(
    'title' => t('Membership Owner ID'),
    'help' => t('The numeric ID Contact that owns the membership'),
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

  //BOOLEAN : IS test membership
  $data['civicrm_membership']['is_test'] = array(
    'title' => t('Test Membership?'),
    'help' => t('Is this a test membership?'),
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
  //BOOLEAN : IS Pay later
  $data['civicrm_membership']['is_pay_later'] = array(
    'title' => t('Is Pay Later'),
    'help' => t('Is the Membership pay later?'),
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
  // Link to Campaign Table
  $data['civicrm_membership']['campaign_id'] = array(
    'title' => t('Campaign'),
    'help' => t('Membership -> Campaign'),
    'relationship' => array(
      'base' => 'civicrm_campaign',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Membership -> Campaign'),
    ),
  );

  /*
     * Add support for CivicRM groups table
     */


  $data['civicrm_group']['table']['group'] = t('CiviCRM Groups');


  $data['civicrm_group']['table']['join']['civicrm_contact'] = array(
    'left_table' => 'civicrm_group_contact',
    'left_field' => 'group_id',
    'field' => 'id',
  );

  $data['civicrm_group_contact']['table']['join']['civicrm_contact'] = array(
    // Directly links to group table
    'left_field' => 'id',
    'field' => 'contact_id',
  );

  $data['civicrm_group_contact']['table']['group'] = t('CiviCRM Groups');
  $data['civicrm_group_status']['table']['group'] = t('Contact Group Status');

  $data['civicrm_group_contact']['status'] = array(
    'title' => t('Contact Status'),
    'help' => t('The group status of the contact'),
    'field' => array(
      'handler' => 'civicrm_handler_filter_group_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_group_status',
      'allow empty' => TRUE,
    ),
  );

  // CiviCRM Groups - FIELDS

  // Numeric Group ID
  $data['civicrm_group_contact']['status'] = array(
    'title' => t('Contact Status'),
    'help' => t('The group status of the contact'),
    'field' => array(
      'handler' => 'civicrm_handler_filter_group_status',
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_group_status',
      'allow empty' => TRUE,
    ),
  );

  //CiviCRM Groups - FIELDS

  //Numeric Group ID
  $data['civicrm_group']['id'] = array(
    'title' => t('Group ID'),
    'help' => t('The numeric ID of the Group'),
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
  //Is Group Active?
  $data['civicrm_group']['is_active'] = array(
    'title' => t('Is Active'),
    'help' => t('Is the Group Active'),
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



  $data['civicrm_group']['title'] = array(
    'title' => t('Title'),
    'help' => t('Group Title'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_group_title',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['civicrm_group']['description'] = array(
    'title' => t('Description'),
    'help' => t('Description of the Group'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  /*
     * Add support for CivicRM tags
     */

  $data['civicrm_tag']['table']['group'] = t('CiviCRM Tags');

  $data['civicrm_tag']['table']['join']['civicrm_contact'] = array(
    'left_table' => 'civicrm_entity_tag',
    'left_field' => 'tag_id',
    'field' => 'id',
  );

  $data['civicrm_entity_tag']['table']['join']['civicrm_contact'] = array(
    // Directly links to group table
    'left_field' => 'id',
    'field' => 'entity_id',
  );


  //CiviCRM Groups - FIELDS

  //Numeric Group ID
  $data['civicrm_tag']['id'] = array(
    'title' => t('Tag ID'),
    'help' => t('The numeric ID of the Group'),
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

  $data['civicrm_tag']['name'] = array(
    'title' => t('Name'),
    'help' => t('Name of Tag'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_tag_title',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  $data['civicrm_tag']['description'] = array(
    'title' => t('Description'),
    'help' => t('Description of the Tag'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //------------------------------------------------------
  // CivicRM Personal Campaign Pages tables
  //------------------------------------------------------

  $data['civicrm_pcp']['table']['group'] = t('CiviCRM Personal Campaign Pages');

  $data['civicrm_pcp']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Personal Campaign Pages'),
    'help' => t("View displays CiviCRM Personal Campaign Pages, with connection to contacts."),
  );

  $data['civicrm_pcp']['table']['join'] = array(
    // Makes PCP fields available to Contact views.
    'civicrm_contact' => array(
      'left_field' => 'id',
      'field' => 'contact_id',
    ),
  );

  // PCP FIELDS

  // Numeric PCP ID
  $data['civicrm_pcp']['id'] = array(
    'title' => t('PCP ID'),
    'help' => t('The numeric ID of the Personal Campaign Page.'),
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

  // PCP Text Link
  $data['civicrm_pcp']['url'] = array(
    'title' => t('URL'),
    'help' => t('Generates URLs for various PCP actions.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_link_pcp',
    ),
  );

  // PCP status
  $data['civicrm_pcp']['status'] = array(
    'title' => t('Page Status'),
    'real field' => 'status_id',
    'help' => t('The Status of this Personal Contribution Page'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pcp_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pcp_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // PCP title
  $data['civicrm_pcp']['title'] = array(
    'title' => t('Title'),
    'help' => t('The title of this Personal Contribution Page'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  // introductory text
  $data['civicrm_pcp']['intro_text'] = array(
    'title' => t('Intro text'),
    'help' => t('Introductory welcome text'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  // page text
  $data['civicrm_pcp']['page_text'] = array(
    'title' => t('Page text'),
    'help' => t('Page text (your message)'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  // text for donate link
  $data['civicrm_pcp']['donate_link_text'] = array(
    'title' => t('Text for donate link'),
    'help' => t('Text for donate link'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  // contribution page
  $data['civicrm_pcp']['contribution_page'] = array(
    'title' => t('Contribution Page'),
    'real field' => 'contribution_page_id',
    'help' => t('The Contribution Page with which this Personal Campaign Page is associated.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_contribution_page',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
      'numeric' => TRUE,
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_contribution_page',
      'allow_empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //BOOLEAN : IS Thermometer
  $data['civicrm_pcp']['is_thermometer'] = array(
    'title' => t('Is Thermometer'),
    'help' => t('Does the page use a progress indicator?'),
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

  //BOOLEAN : Is Honor Roll
  $data['civicrm_pcp']['is_honor_roll'] = array(
    'title' => t('Is Honor Roll'),
    'help' => t('Does the page use an Honor Roll?'),
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

  // fundraising goal
  $data['civicrm_pcp']['goal_amount'] = array(
    'title' => t('Goal Amount'),
    'help' => t('Goal Amount.'),
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

  // amount raised
  $data['civicrm_pcp']['raised_amount'] = array(
    'title' => t('Amount Raised'),
    'help' => t('Amount raised through this Personal Campaign Page'),
    'real field' => id,
    'field' => array(
      'handler' => 'civicrm_handler_field_pcp_raised_amount',
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

  // referer (not really sure what this is, but...)
  $data['civicrm_pcp']['referer'] = array(
    'title' => t('Referer'),
    'help' => t('Referer'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //BOOLEAN : Is Active
  $data['civicrm_pcp']['is_active'] = array(
    'title' => t('Is Active'),
    'help' => t('Is this page active?'),
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

  // currency
  $data['civicrm_pcp']['currency'] = array(
    'title' => t('Currency'),
    'help' => t('Currency'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['civicrm_pcp']['image_url'] = array(
    'title' => t('Image URL'),
    'real field' => id,
    'help' => t('URL for the image the user uploaded when creating this page'),
    'field' => array(
      'handler' => 'civicrm_handler_field_file',
    ),
  );


  /*
     * CivicRM Mailings tables
     */

  $data['civicrm_mailing']['table']['group'] = t('CiviCRM Mailings');

  $data['civicrm_mailing']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Mailings'),
    'help' => t("View displays CiviCRM Mailing Results."),
  );

  $data['civicrm_mailing_event_queue']['table']['join'] = array(
    // Directly links to contact table - link A
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_job',
      'left_field' => 'id',
      'field' => 'job_id',
    ),
  );



  //CiviCRM Mailing - FIELDS
  // expose the ID to Views to allow users to drill down to a specific mailing.
  $data['civicrm_mailing']['id'] = array(
    'title' => t('ID'),
    'help' => t('The mailing\'s ID in the database.'),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
    ),
    'field' => array(
      'handler' => 'views_handler_field',
    ),
  );

  //Mailing Name
  $data['civicrm_mailing']['name'] = array(
    'title' => t('Name'),
    'help' => t('Title of the mailing'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_mailing_name',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Mailing Subject line
  $data['civicrm_mailing']['subject'] = array(
    'title' => t('Subject Line'),
    'help' => t('Subject Line of the mailing'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Mailing Body plaintext
  $data['civicrm_mailing']['body_text'] = array(
    'title' => t('Body - Text'),
    'help' => t('Plaintext mailing message'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Mailing Body HTML
  $data['civicrm_mailing']['body_html'] = array(
    'title' => t('Body - HTML'),
    'help' => t('HTML mailing message'),
    'field' => array(
      'handler' => 'civicrm_handler_field_markup',
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

  //Mailing From e-mail
  $data['civicrm_mailing']['from_email'] = array(
    'title' => t('From Email'),
    'help' => t('From Email of the mailing'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //Mailing reply to e-mail
  $data['civicrm_mailing']['replyto_email'] = array(
    'title' => t('Reply to Email'),
    'help' => t('Reply Email of the mailing'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //Does the mailng track click throughs
  $data['civicrm_mailing']['url_tracking'] = array(
    'title' => t('Track URLs?'),
    'help' => t('Does the mailing track click throughs?'),
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
  //Does the e-mail track opens
  $data['civicrm_mailing']['open_tracking'] = array(
    'title' => t('Track Opens?'),
    'help' => t('Does the mailing track Open Rate?'),
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
  //has mailing been sent
  $data['civicrm_mailing']['is_completed'] = array(
    'title' => t('Complete?'),
    'help' => t('Is the mailing completely sent?'),
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
  // Link to Campaign Table
  $data['civicrm_mailing']['campaign_id'] = array(
    'title' => t('Campaign'),
    'help' => t('Mailing -> Campaign'),
    'relationship' => array(
      'base' => 'civicrm_campaign',
      'field' => 'id',
      'handler' => 'views_handler_relationship',
      'label' => t('Mailing -> Campaign'),
    ),
  );


  // mailing_component table aliased as mailing_header
  $data['civicrm_mailing_header']['table']['group'] = ts('CiviCRM Mailings');

  $data['civicrm_mailing_header']['table']['join'] = array(
    // link to civicrm_mailing
    'civicrm_mailing' => array(
      'left_field' => 'header_id',
      'table' => 'civicrm_mailing_component',
      'field' => 'id',
    ),
  );

  $data['civicrm_mailing_header']['body_html'] = array(
    'title' => t('Mailing Header - HTML'),
    'help' => t('The HTML header for this mailing.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => FALSE,
    ),
  );

  $data['civicrm_mailing_header']['body_text'] = array(
    'title' => t('Mailing Header - Plaintext'),
    'help' => t('The plaintext header for this mailing.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
  );


  // mailing_component table aliased as mailing_footer
  $data['civicrm_mailing_footer']['table']['group'] = ts('CiviCRM Mailings');

  $data['civicrm_mailing_footer']['table']['join'] = array(
    // link to civicrm_mailing
    'civicrm_mailing' => array(
      'left_field' => 'footer_id',
      'table' => 'civicrm_mailing_component',
      'field' => 'id',
    ),
  );

  $data['civicrm_mailing_footer']['body_html'] = array(
    'title' => t('Mailing Footer - HTML'),
    'help' => t('The HTML footer for this mailing.'),
    'field' => array(
      'handler' => 'civicrm_handler_field_markup',
      'click sortable' => FALSE,
    ),
  );

  $data['civicrm_mailing_footer']['body_text'] = array(
    'title' => t('Mailing Footer - Plaintext'),
    'help' => t('The plaintext footer for this mailing.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
  );


  //Mailing Job table
  $data['civicrm_mailing_job']['table']['group'] = ts('CiviCRM Mailing Job');

  $data['civicrm_mailing_job']['table']['join'] = array(
    // link to civicrm_mailing
    'civicrm_mailing' => array(
      'left_field' => 'id',
      'field' => 'mailing_id',
    ),
  );

  //When is the mailing sceduled to go out
  $data['civicrm_mailing_job']['scheduled_date'] = array(
    'title' => t('Scheduled Date'),
    'help' => t('The Mailing\'s Scheduled Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_mailing_job'], array('title' => 'Scheduled Date',
      'name' => 'scheduled_date',
    ));
  //When did the mailing start to send
  $data['civicrm_mailing_job']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The Mailing\'s Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_mailing_job'], array('title' => 'Start Date',
      'name' => 'start_date',
    ));
  //Mailing finish date
  $data['civicrm_mailing_job']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The Mailing\'s End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_mailing_job'], array('title' => 'End Date',
      'name' => 'end_date',
    ));


  //Is this a test mailing
  $data['civicrm_mailing_job']['is_test'] = array(
    'title' => t('Is Test?'),
    'help' => t('Is this the test mailing?'),
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
  //Mailing table that tracks who forwarded the e-mail
  $data['civicrm_mailing_event_forward']['table']['group'] = ts('CiviCRM Mailing Forwards');

  $data['civicrm_mailing_event_forward']['table']['join'] = array(
    // links through civicrm_mail_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );
  //time stamp of eash individual forward
  $data['civicrm_mailing_event_forward']['time_stamp'] = array(
    'title' => t('Date forwaded'),
    'help' => t('Date user forwarded the e-mail'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  //Tracks who click through, traces each individual click through
  $data['civicrm_mailing_event_trackable_url_open']['table']['group'] = ts('CiviCRM Mailing Click Throughs');

  $data['civicrm_mailing_event_trackable_url_open']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );
  //timestamp of click through
  $data['civicrm_mailing_event_trackable_url_open']['time_stamp'] = array(
    'title' => t('Date of click through'),
    'help' => t('Date user clicked a link in the e-mail'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  //ID of click through event
  $data['civicrm_mailing_event_trackable_url_open']['id'] = array(
    'title' => t('Mailing click through ID'),
    'help' => t('The numeric ID click through record'),
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
  //Track the click through's URL
  $data['civicrm_mailing_trackable_url']['table']['group'] = ts('CiviCRM Mailing Click Throughs URL');

  $data['civicrm_mailing_trackable_url']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_trackable_url_open',
      'left_field' => 'trackable_url_id',
      'field' => 'id',
    ),
  );
  $data['civicrm_mailing_trackable_url']['url'] = array(
    'title' => t('URL'),
    'help' => t('What URL was clicked'),
    'field' => array(
      'handler' => 'views_handler_field_url',
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



  $data['civicrm_mailing_event_bounce']['table']['group'] = ts('CiviCRM Mailing Bounced');

  $data['civicrm_mailing_event_bounce']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );

  $data['civicrm_mailing_event_bounce']['time_stamp'] = array(
    'title' => t('Date of bounce'),
    'help' => t('Date user e-mail bounced'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  $data['civicrm_mailing_event_bounce']['id'] = array(
    'title' => t('Mailing bounced ID'),
    'help' => t('The numeric ID of the bounce record'),
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

  $data['civicrm_mailing_event_bounce']['bounce_reason'] = array(
    'title' => t('Reason'),
    'help' => t('Reason the e-mail bounced'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  $data['civicrm_mailing_event_delivered']['table']['group'] = ts('CiviCRM Mailing  Delivered');

  $data['civicrm_mailing_event_delivered']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );

  $data['civicrm_mailing_event_delivered']['time_stamp'] = array(
    'title' => t('Date of delivery'),
    'help' => t('Date user e-mail delivered'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  $data['civicrm_mailing_event_delivered']['id'] = array(
    'title' => t('Mailing delivered ID'),
    'help' => t('The numeric ID of the delivery record'),
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


  $data['civicrm_mailing_event_opened']['table']['group'] = ts('CiviCRM Mailing Opened');

  $data['civicrm_mailing_event_opened']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );

  $data['civicrm_mailing_event_opened']['time_stamp'] = array(
    'title' => t('Date e-mail opened'),
    'help' => t('Date e-mail opened'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  $data['civicrm_mailing_event_opened']['id'] = array(
    'title' => t('Mailing opened ID'),
    'help' => t('The numeric ID of the opened record'),
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


  $data['civicrm_mailing_event_reply']['table']['group'] = ts('CiviCRM Mailing Replied');

  $data['civicrm_mailing_event_reply']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );

  $data['civicrm_mailing_event_reply']['time_stamp'] = array(
    'title' => t('Date of reply'),
    'help' => t('Date user replied'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );
  $data['civicrm_mailing_event_reply']['id'] = array(
    'title' => t('Mailing reply ID'),
    'help' => t('The numeric ID of the reply record'),
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


  $data['civicrm_mailing_event_unsubscribe']['table']['group'] = ts('CiviCRM Mailing Unsubscribe');

  $data['civicrm_mailing_event_unsubcribe']['table']['join'] = array(
    // link to civicrm_mailing through civicrm_mailing_event_queue
    'civicrm_mailing' => array(
      'left_table' => 'civicrm_mailing_event_queue',
      'left_field' => 'id',
      'field' => 'event_queue_id',
    ),
  );

  $data['civicrm_mailing_event_unsubscribe']['time_stamp'] = array(
    'title' => t('Date of unsubscribe'),
    'help' => t('Date user unsubscribed'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  $data['civicrm_mailing_event_unsubscribe']['id'] = array(
    'title' => t('Mailing unsubscribeded ID'),
    'help' => t('The numeric ID of the unsubscribe record'),
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
   *CiviCRM Campaign Base Table
   */

  $data['civicrm_campaign']['table']['group'] = t('CiviCRM Campaigns');

  $data['civicrm_campaign']['table']['base'] = array(
    // Governs the whole mozilla
    'field' => 'id',
    'title' => t('CiviCRM Campaigns'),
    'help' => t("View displays CiviCRM Campaigns Data"),
  );
  // Campaign ID
  $data['civicrm_campaign']['id'] = array(
    'title' => t('Campaign ID'),
    'help' => t('The numeric ID of the campaign'),
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
  // Campaign's Name
  $data['civicrm_campaign']['name'] = array(
    'title' => t('Name'),
    'help' => t('Campaing\'s name'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // Campaign Title
  $data['civicrm_campaign']['title'] = array(
    'title' => t('Title'),
    'help' => t('Campaing\'s title'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  //Campaign Description
  $data['civicrm_campaign']['description'] = array(
    'title' => t('Description'),
    'help' => t('Campaing\'s description'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
  );
  //Mailing finish date
  $data['civicrm_campaign']['start_date'] = array(
    'title' => t('Start Date'),
    'help' => t('The Campaign\'s Start Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_campaign'], array('title' => 'Campaign Start Date',
      'name' => 'start_date',
    ));

  //Mailing finish date
  $data['civicrm_campaign']['end_date'] = array(
    'title' => t('End Date'),
    'help' => t('The Campaign\'s End Date'),
    'field' => array(
      'handler' => 'civicrm_handler_field_datetime',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_datetime',
    ),
    'sort' => array(
      'handler' => 'civicrm_handler_sort_date',
    ),
  );

  civicrm_views_add_date_arguments($data['civicrm_campaign'], array('title' => 'Campaign End Date',
      'name' => 'end_date',
    ));

  //Campaign Type
  $data['civicrm_campaign']['campaign_type'] = array(
    'title' => t('Campaign Type'),
    'real field' => 'campaign_type_id',
    'help' => t('The Type of Campaign, ie Direct Mail, Constituent Engagement etc'),
    'field' => array(
      'handler' => 'civicrm_handler_field_campaign_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_campaign_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  //Campaign Type
  $data['civicrm_campaign']['campaign_status'] = array(
    'title' => t('Campaign Status'),
    'real field' => 'status_id',
    'help' => t('The Status of the Campaign, ie Completed, Cancelled etc'),
    'field' => array(
      'handler' => 'civicrm_handler_field_campaign_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_campaign_status',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );
  // Parent Campaign ID
  $data['civicrm_campaign']['parent_id'] = array(
    'title' => t('Parent Campaign ID'),
    'help' => t('The ID of the campaign\'s parent campaign'),
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
  //BOOLEAN : IS Deceased
  $data['civicrm_campaign']['is_active'] = array(
    'title' => t('Is Active'),
    'help' => t('Is the campaign active ?'),
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
  // Campaign's General Goal
  $data['civicrm_campaign']['goal_general'] = array(
    'title' => t('General Goal'),
    'help' => t('Campaing\'s general goal'),
    'field' => array(
      'handler' => 'views_handler_field',
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
  // Campaign ID
  $data['civicrm_campaign']['goal_revenue'] = array(
    'title' => t('Campaign Monetary Goal'),
    'help' => t('The Monetary goal of the campaign'),
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


  /*
    * UF Match table - expose drupal ID
    */

  $data['civicrm_uf_match']['table']['group'] = t("CiviCRM Drupal match table");
  $data['civicrm_uf_match']['uf_id'] = array(
    'title' => t('Drupal User ID'),
    'help' => t('The user id of the User matched to a CiviCRM Contact'),
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

  /*
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
      'handler' => 'civicrm_handler_field_grant_status',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_grant_status',
      'allow empty' => TRUE,
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
      'handler' => 'civicrm_handler_field_grant_type',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_grant_type',
      'allow empty' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  //Grant Amount
  $data['civicrm_grant']['amount_total'] = array(
    'title' => t('Grant Amount'),
    'help' => t('Requested grant amount, in default currency'),
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

  //Grant Amount Requested
  $data['civicrm_grant']['amount_total'] = array(
    'title' => t('Amount Requested'),
    'help' => t('Grant amount requested'),
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

  //Grant Rationale
  $data['civicrm_grant']['rationale'] = array(
    'title' => t('Rationale'),
    'help' => t('Grant rationale'),
    'field' => array(
      'handler' => 'views_handler_field',
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

  //Activity Text Link
  $data['civicrm_activity']['text_link'] = array(
    'field' => array(
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various activity actions.'),
      'handler' => 'civicrm_handler_field_link_activity',
    ),
  );


  //Relationship Text Link
  $data['civicrm_relationship']['text_link'] = array(
    'field' => array(
      'title' => t('Text Link'),
      'help' => t('Creates a Text Link for various relationship actions.'),
      'handler' => 'civicrm_handler_field_link_relationship',
    ),
  );

  $query = "select id, extends, extends_entity_column_value, style from civicrm_custom_group where is_active = 1";

  $dao = CRM_Core_DAO::executeQuery($query);


  while ($dao->fetch()) {
    // call getTree using $dao->id as groupID and $dao->extends as entityType

    $data = civicrm_views_custom_data_cache($data, $dao->extends, $dao->id, $dao->extends_entity_column_value, $dao->style);
  }



  return $data;
}



// RETURNS A LINK TO A CIVICRM PATH
function civicrm_views_href($text, $path, $query) {
  civicrm_initialize();
  require_once 'CRM/Utils/System.php';
  return CRM_Utils_System::href($text, $path, $query);
}

// Creates new View fields from CiviCRM fields
function civicrm_views_add_fields(&$fields, &$data, &$skipFields, $tableName) {
  foreach ($fields as $name => $value) {
    if (CRM_Utils_Array::value($name, $skipFields) ||
      (isset($value['where']) &&
        substr($value['where'], 0, strlen($tableName) + 1) != "{$tableName}."
      )
    ) {
      continue;
    }

    $type = CRM_Utils_Array::value('type', $value, 'String');

    $data[$value['name']] = array(
      'title' => $value['title'],
      'help' => $value['title'],
      'field' => array(
        'handler' => civicrm_get_field_handler($type),
        'click sortable' => TRUE,
      ),
      'sort' => array(
        'handler' => civicrm_get_sort_handler($type),
      ),
      'filter' => array(
        'handler' => civicrm_get_filter_handler($type),
      ),
      'argument' => array(
        'handler' => civicrm_get_argument_handler($type),
      ),
    );

    // For date fields add in 6 arguments
    // not sure how its numeric here, but leaving it as is for now
    if ($type == 4) {
      civicrm_views_add_date_arguments($data, $value);
    }
  }
}

/*
 * Function adds 6 date arguments to a date field
 *
 */
function civicrm_views_add_date_arguments(&$data, $value) {

  $data[$value['name'] . '_full'] = array(
    'title' => $value['title'],
    'help' => t('In the form of CCYYMMDD.'),
    'argument' => array(
      'field' => $value['name'],
      'handler' => 'views_handler_argument_civicrm_fulldate',
    ),
  );
  $data[$value['name'] . '_year_month'] = array(
    'title' => t('%title year + month', array('%title' => $value['title'])),
    'help' => t('In the form of YYYYMM.'),
    'argument' => array(
      'field' => $value['name'],
      'handler' => 'views_handler_argument_civicrm_year_month',
    ),
  );
  $data[$value['name'] . '_year'] = array(
    'title' => t('%title year', array('%title' => $value['title'])),
    'help' => t('In the form of YYYY.'),
    'argument' => array(
      'field' => $value['name'],
      'handler' => 'views_handler_argument_civicrm_year',
    ),
  );
  $data[$value['name'] . '_month'] = array(
    'title' => t('%title month', array('%title' => $value['title'])),
    'help' => t('In the form of MM (01 - 12).'),
    'argument' => array(
      'field' => $value['name'],
      'handler' => 'views_handler_argument_civicrm_month',
    ),
  );
  $data[$value['name'] . '_day'] = array(
    'title' => t('%title day', array('%title' => $value['title'])),
    'help' => t('In the form of DD (01 - 31).'),
    'argument' => array(
      'field' => $value['name'],
      'handler' => 'views_handler_argument_civicrm_day',
    ),
  );
  $data[$value['name'] . '_week'] = array(
    'title' => t('%title week', array('%title' => $value['title'])),
    'help' => t('In the form of WW (01 - 53).'),
    'argument' => array(
      'field' => $value['name'],
      'handler' => 'views_handler_argument_civicrm_week',
    ),
  );
}

function civicrm_views_custom_data_cache($data, $entity_type, $groupID, $subType, $style) {

  //Feels a bit hacky this next 10 lines but it was the only way I could get getTree to play nice with the data
  $config = &CRM_Core_Config::singleton();
  if ($style == 'Inline') {
    $tree = CRM_Core_BAO_CustomGroup::getTree($entity_type, CRM_Core_DAO::$_nullObject, NULL, NULL, $subType, NULL);
  }
  else {
    $tree = CRM_Core_BAO_CustomGroup::getTree($entity_type, CRM_Core_DAO::$_nullObject, NULL, $groupID);
  }

  switch ($entity_type) {
    case "Contact":
    case "Individual":
    case "Household":
    case "Organization":
    case "Address":
    case "Group":
      $jointable = 'civicrm_contact';
      break;

    case "Event":
      $jointable = 'civicrm_event';
      break;

    case "Participant":
      $jointable = 'civicrm_participant';
      break;

    case "Contribution":
      $jointable = 'civicrm_contribution';
      break;

    case "Activity":
      $jointable = 'civicrm_activity';
      break;

    case "Relationship":
      $jointable = 'civicrm_relationship';
      break;

    case "Membership":
      $jointable = 'civicrm_membership';
      break;

    case "Grant":
      $jointable = 'civicrm_grant';
      break;

    case "Campaign":
    case "Survey":
      $jointable = 'civicrm_campaign';
      break;
  }


  foreach ($tree as $groupkey => $currentgroup) {
    if ($groupkey == 'info') {
      return $data;
    // dodges an invalid argument call after all the groups go through // have a look at what $tree outputs to to see why.
    }
    $data[$currentgroup['table_name']]['table']['group'] = t('CiviCRM Custom: ') . $currentgroup['title'];

    // Join this table to Contacts
    $data[$currentgroup['table_name']]['table']['join'] = array(
      // Directly links to join table.
      $jointable => array(
        'left_field' => 'id',
        'field' => 'entity_id',
      ),
    );
    // expose custom data to users view - note I tested with contact & membership fields which worked
    // event won't work due to missing relationship in chain.
    // 'contribution' & 'activity' not currently related to users so excluded
    if ($jointable != 'civicrm_event' && $jointable != 'civicrm_activity' && $jointable != 'civicrm_contribution') {
      $data[$currentgroup['table_name']]['table']['join']['users'] = array(
        'left_table' => $jointable,
        'left_field' => 'id',
        'field' => 'entity_id',
      );
    }
    $currentgroupfields = $currentgroup['fields'];
    $customHTMLTypes = array('Select', 'Multi-Select', 'AdvMulti-Select', 'Radio', 'CheckBox',
      'Select State/Province', 'Select Country', 'Multi-Select Country',
      'Multi-Select State/Province', 'Autocomplete-Select',
    );
    $customMultiValueHTMLTypes = array('Multi-Select', 'AdvMulti-Select', 'CheckBox', 'Multi-Select Country',
      'Multi-Select State/Province',
    );
    foreach ($currentgroupfields as $key => $currentfield) {
      if (empty($currentfield['help_post'])) {
        $currentfield['help_post'] = t('Custom Data Field');
      }
      // Populates help text so there isn't miles of error: Missing Help


      if (in_array($currentfield['html_type'],
          $customHTMLTypes
        )) {
        $handler = 'civicrm_handler_field_custom';
      }
      elseif ($currentfield['html_type'] == 'File') {
        $handler = 'civicrm_handler_field_custom_file';
      }
      else {
        $handler = civicrm_get_field_handler($currentfield['data_type']);
      }

      if (!empty($currentfield['option_group_id']) && in_array($currentfield['html_type'], $customMultiValueHTMLTypes)) {
        $filter_handler = 'civicrm_handler_filter_custom_option';
      }
      elseif (!empty($currentfield['option_group_id'])) {
        $filter_handler = 'civicrm_handler_filter_custom_single_option';
      }
      else {
        $filter_handler = civicrm_get_filter_handler($currentfield['data_type']);
      }

      //Create the Views Field
      $data[$currentgroup['table_name']][$currentfield['column_name']] = array(
        'title' => $currentfield['label'],
        'help' => $currentfield['help_post'],
        'field' => array(
          'handler' => $handler,
          'click sortable' => TRUE,
        ),
        'argument' => array(
          'handler' => civicrm_get_argument_handler($currentfield['data_type']),
        ),
        'filter' => array(
          'handler' => $filter_handler,
        ),
        'sort' => array(
          'handler' => civicrm_get_sort_handler($currentfield['data_type']),
        ),
      );
      //For date fields add in 6 arguments
      if ($currentfield['data_type'] == 'Date') {
        //@TODO  Still need to get the field under it's respecitve group, I may e able to set the civicrm_views_add_date_arguments() function with a group variable and default it to null
        $value          = array();
        $value['title'] = $currentfield['label'];
        $value['name']  = $currentfield['column_name'];
        civicrm_views_add_date_arguments($data[$currentgroup['table_name']], $value);
      }
    }
  }
  return $data;
}


//Getting the proper handlers by checking against the field's data_type
function civicrm_get_field_handler($type) {
  switch ($type) {
    case "String":
    case "Memo":
      return 'civicrm_handler_field_markup';

    case "Float":
    case "Int":
      return 'views_handler_field_numeric';

    case "Date":
      return 'civicrm_handler_field_datetime';

    case "Boolean":
      return 'views_handler_field_boolean';

    case "StateProvince":
      return 'civicrm_handler_field_state';

    case "Country":
      return 'civicrm_handler_field_country';

    case "County":
      return 'civicrm_handler_field_county';

    default:
      return 'views_handler_field';
  }
}

function civicrm_get_argument_handler($type) {
  switch ($type) {
    case "String":
    case "Memo":
      return 'views_handler_argument';

    case "Float":
    case "Int":
      return 'views_handler_argument_numeric';

    case "Date":
      return 'views_handler_argument_date';

    case "Boolean":
      return 'views_handler_argument';

    case "StateProvince":
      return 'views_handler_argument';

    case "Country":
      return 'views_handler_argument';

    case "County":
      return 'views_handler_argument';

    default:
      return 'views_handler_argument';
  }
}

function civicrm_get_filter_handler($type) {
  switch ($type) {
    case "String":
    case "Memo":
      return 'views_handler_filter_string';

    case "Float":
    case "Int":
      return 'views_handler_filter_numeric';

    case "Date":
      return 'civicrm_handler_filter_datetime';

    case "Boolean":
      return 'views_handler_filter_boolean_operator';

    case "StateProvince":
      return 'civicrm_handler_filter_state';

    case "Country":
      return 'civicrm_handler_filter_country';

    case "County":
      return 'civicrm_handler_filter_county';

    default:
      return 'views_handler_filter_string';
  }
}

function civicrm_get_sort_handler($type) {
  switch ($type) {
    case "String":
    case "Memo":
    case "Float":
    case "Int":
      return 'views_handler_sort';

    case "Date":
      return 'views_handler_sort_date';

    case "Boolean":
      return 'views_handler_sort';

    case "StateProvince":
      return 'views_handler_sort';

    case "Country":
      return 'views_handler_sort';

    case "County":
      return 'views_handler_sort';

    default:
      return 'views_handler_sort';
  }
}

//Implementation of hook_date_api_fields
function civicrm_date_api_fields($field) {
  $values = array(
    // The type of date: DATE_UNIX, DATE_ISO, DATE_DATETIME.
    'sql_type' => DATE_DATETIME,
    // Timezone handling options: 'none', 'site', 'date', 'utc'.
    'tz_handling' => 'site',
    // Needed only for dates that use 'date' tz_handling.
    'timezone_field' => '',
    // Needed only for dates that use 'date' tz_handling.
    'offset_field' => '',
    // Array of "table.field" values for related fields that should be
    // loaded automatically in the Views SQL.
    'related_fields' => array(),
    // Granularity of this date field's db data.
    'granularity' => array('year', 'month', 'day', 'hour', 'minute', 'second'),
  );

  switch ($field) {
    case 'civicrm_event.start_date':
    case 'civicrm_event.end_date':
    case 'civicrm_event.registration_start_date':
    case 'civicrm_event.registration_end_date':
    case 'civicrm_mailing_job.scheduled_date':
    case 'civicrm_mailing_job.start_date':
    case 'civicrm_mailing_job.end_date':
    case 'civicrm_activity.activity_date_time':
    case 'civicrm_campaign.start_date':
    case 'civicrm_campaign.end_date':
      return $values;
  }
}

/**
 * Implement hook_date_api_tables().
 */
function civicrm_date_api_tables() {
  return array('civicrm_mailing_job',
    'civicrm_event',
    'civicrm_activity',
    'civicrm_campaign',
  );
}

// Telling Views where to get the Handlers
function civicrm_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'civicrm') . '/modules/views/civicrm',
    ),
    'handlers' =>
    //FIELD HANDLAAARRRRS! Field Handlers!
    array(
      // LINK FIELDS GO HERE
      'civicrm_handler_field_link_contact' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_link_event' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_link_participant' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_link_contribution' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_link_activity' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_link_relationship' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_contact_link' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_link_pcp' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_gender' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_phone' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_phone_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_location_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_state' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_country' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_county' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_email' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_datetime' => array(
        'parent' => 'views_handler_field_date',
      ),
      'civicrm_handler_field_event_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_event_link' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_event_price_set' => array(
        'parent' => 'views_handler_field_prerender_list',
      ),
      'civicrm_handler_field_participant_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_participant_role' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_contribution_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_contribution_page' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_payment_instrument' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_contribution_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_activity_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_activity_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_priority' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_encounter_medium' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_relationship_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_drupalid' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_membership_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_membership_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_relationship_contact2users' => array(
        'parent' => 'views_handler_relationship',
      ),
      'civicrm_handler_field_markup' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_custom' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_custom_file' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_campaign_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_campaign_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_grant_type' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_grant_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_pcp_status' => array(
        'parent' => 'views_handler_field',
      ),
      'civicrm_handler_field_pcp_raised_amount' => array(
        'parent' => 'views_handler_field_numeric',
      ),
      'civicrm_handler_field_file' => array(
        'parent' => 'views_handler_field',
      ),
      //FILTER HANDDLEAARRRS! Filter Handlers!
      'civicrm_handler_filter_datetime' => array(
        'parent' => 'views_handler_filter_date',
      ),
      'civicrm_handler_filter_gender' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_phone_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_location_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_state' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_country' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_county' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_event_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_participant_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_group_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_group_title' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_tag_title' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_participant_role' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_contribution_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_contribution_page' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_payment_instrument' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_contribution_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_activity_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_activity_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_priority' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_encounter_medium' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_contact_date_api_filter_handler' => array(
        'parent' => 'date_api_filter_handler',
      ),
      'civicrm_handler_filter_relationship_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_contact_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_membership_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_membership_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_mailing_name' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_custom_option' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_custom_single_option' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_campaign_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_campaign_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_grant_type' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_grant_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'civicrm_handler_filter_pcp_status' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      // ARGUMENT HANDLERS
      'civicrm_handler_argument_date_api_contact' => array(
        'parent' => 'date_api_argument_handler',
      ),
      'views_handler_argument_civicrm_fulldate' => array(
        'parent' => 'views_handler_argument_date',
      ),
      'views_handler_argument_civicrm_year' => array(
        'parent' => 'views_handler_argument_date',
      ),
      'views_handler_argument_civicrm_year_month' => array(
        'parent' => 'views_handler_argument_date',
      ),
      'views_handler_argument_civicrm_month' => array(
        'parent' => 'views_handler_argument_date',
      ),
      'views_handler_argument_civicrm_day' => array(
        'parent' => 'views_handler_argument_date',
      ),
      'views_handler_argument_civicrm_week' => array(
        'parent' => 'views_handler_argument_date',
      ),
      //SORT Handlers
      'civicrm_handler_sort_date' => array(
        'parent' => 'views_handler_sort_date',
      ),
      //Relationship Handlers
      'civicrm_handler_relationship' => array(
        'parent' => 'views_handler_relationship',
      ),
    ),
  );
}

