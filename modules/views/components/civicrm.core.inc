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
    'civicrm_activity_contact' => array(
      'left_field' => 'contact_id',
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

  $data['civicrm_activity']['source_record_id'] = array(
    'title' => t('Record Source'),
    'real field' => 'source_record_id',
    'help' => t('Source record activity'),
  );

  /*
   * This block is commented out for two reasons:
   *  1) The help text is misleading to users; source_record_id is NOT
   *     "the contact who made this activity."
   *  2) It doesn't work; the code attempts to join on the wrong table.
   *
   * TODO: Figure out how to tell Views to join to a table whose identity
   *       isn't known yet. Because the details of the "parent" activity
   *       reside in different tables depending on the value of
   *       activity_type_id, Views needs to be told which table to
   *       join to on the fly.
   *
   *  $data['civicrm_activity']['source_record_id'] = array(
   *    'title' => t('Source Record ID'),
   *    // 'help' => t('The contact who made this activity'),
   *    'help' => t('The parent transaction (e.g. Contribution, Event Registration) if it is not an Activity.'),
   *    'relationship' => array(
   *      'base' => 'civicrm_contact',
   *      'field' => 'id',
   *      'handler' => 'views_handler_relationship',
   *      'label' => t('CiviCRM Source Record Contact'),
   *    ),
   *  );
   */

  //Activity Type
  $data['civicrm_activity']['activity_type'] = array(
    'title' => t('Activity Type'),
    'real field' => 'activity_type_id',
    'help' => t('The Type of Activity'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityType',
      'pseudo args' => array(TRUE, TRUE), // Include cases (if CiviCase is enabled)
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityType',
      'pseudo args' => array(TRUE, TRUE),
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
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityStatus',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'activityStatus',
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
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Activity_DAO_Activity',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Activity_DAO_Activity',
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
  //BOOLEAN : IS AUTO ACTIVITY
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
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Campaign_PseudoConstant',
      'pseudo method' => 'engagementLevel',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'allow empty' => TRUE,
      'pseudo class' => 'CRM_Campaign_PseudoConstant',
      'pseudo method' => 'engagementLevel',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // CRM-11488
  $data['civicrm_activity']['is_current_revision'] = array(
    'title' => t('Current Revision'),
    'help' => t('Use to dedupe Civicase revisions'),
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
  $data['civicrm_activity']['is_deleted'] = array(
    'title' => t('Is Deleted'),
    'help' => t('If the current Activity is deleted. Also used to dedupe Civicase revisions'),
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

  // let's add activity contact table
  $data['civicrm_activity_contact']['table']['group'] = t('CiviCRM Activity Contacts');

  $data['civicrm_activity_contact']['table']['base'] = array(
    'field' => 'id',
    'title' => t('CiviCRM Activity Contacts'),
    'help' => t("View displays CiviCRM Activity, with connection to contacts."),
  );

  // Explain how this table joins to others.
  $data['civicrm_activity_contact']['table']['join'] = array(
    // Directly links to activity table
    'civicrm_activity' => array(
      'left_field' => 'id',
      'field' => 'activity_id',
    ),
  );

  $data['civicrm_activity_contact']['contact_id'] = array(
    'title' => t('Activity Contact ID'),
    'real field' => 'contact_id',
    'help' => t('The contact related to the activity.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => FALSE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'relationship' => array(
      'base' => 'civicrm_contact',
      'base field' => 'id',
      'handler' => 'civicrm_handler_field_activity',
      'label' => t('CiviCRM Activity Contact ID'),
      'relationship table' => 'civicrm_activity',
      'relationship field' => 'activity_id',
      'other_field' => 'contact_id',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
      'allow empty' => TRUE,
    ),
  );

  $data['civicrm_activity_contact']['record_type'] = array(
    'title' => t('Record Type'),
    'real field' => 'record_type_id',
    'help' => t('Record type'),
    'field' => array(
      'handler' => 'civicrm_handler_field_pseudo_constant',
      'click sortable' => TRUE,
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Activity_DAO_ActivityContact',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument',
    ),
    'filter' => array(
      'handler' => 'civicrm_handler_filter_pseudo_constant',
      'pseudo class' => 'CRM_Core_PseudoConstant',
      'pseudo method' => 'get',
      'dao class' => 'CRM_Activity_DAO_ActivityContact',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

}

