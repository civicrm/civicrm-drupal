<?php
// $Id$

#http://pastebin.com/U32jjBVu - D7 has renamed admin settings
function civicrm_engage_admin_settings($form_state = NULL) {
  $form['last_donate'] = array(
    '#type' => 'fieldset',
    '#title' => t('CiviEngage Walklist Last Donate presentation'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['last_donate']['civicrm_engage_no_canvas_period'] = array(
    '#title' => t("Exclusion Period"),
    '#type' => 'textfield',
    '#description' => t('If you wish to leave someone alone for a period after their last donation please enter it here (in months). 0 means disabled'),
    '#default_value' => variable_get('civicrm_engage_no_canvas_period', "0"),
  );
  $form['last_donate']['civicrm_engage_no_canvass_text'] = array(
    '#title' => t("Text when within exclusion period"),
    '#type' => 'textfield',
    '#description' => t('Text to put in if contact not to be canvassed'),
    '#default_value' => variable_get('civicrm_engage_no_canvass_text', "Do Not Canvass"),
  );
  $form['break_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('CiviEngage Walklist data grouping'),
    '#weight' => 0,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['break_settings']['civicrm_engage_groupbreak_odd_even'] = array(
    '#title' => t("Group break on odds / evens"),
    '#type' => 'checkbox',
    '#description' => t('Do you want a group-break between each side of the street'),
    '#default_value' => variable_get('civicrm_engage_groupbreak_odd_even', "1"),
  );
  $form['break_settings']['civicrm_engage_groupbreak_street'] = array(
    '#title' => t("Group break on street "),
    '#type' => 'checkbox',
    '#description' => t('Do you want a group-break between each street'),
    '#default_value' => variable_get('civicrm_engage_groupbreak_street', "1"),
  );
  $form['break_settings']['civicrm_engage_groupbreak_city'] = array(
    '#title' => t("Group break on city"),
    '#type' => 'checkbox',
    '#description' => t('Do you want a group-break between each city'),
    '#default_value' => variable_get('civicrm_engage_groupbreak_city', "1"),
  );
  $form['break_settings']['civicrm_engage_groupbreak_zip'] = array(
    '#title' => t("Group break on zip code"),
    '#type' => 'checkbox',
    '#description' => t('Do you want a group-break between each side of the street'),
    '#default_value' => variable_get('civicrm_engage_groupbreak_zip', "1"),
  );
  $form['break_settings']['civicrm_engage_lines_per_group'] = array(
    '#title' => t("Row limit per group"),
    '#type' => 'textfield',
    '#description' => t('Limit rows per group - ie. if you want to print out in blocks of 6'),
    '#default_value' => variable_get('civicrm_engage_lines_per_group', "6"),
  );
  $form['break_settings']['civicrm_engage_group_per_page'] = array(
    '#title' => t("New page for each group?"),
    '#type' => 'checkbox',
    '#description' => t('Do you want each group to have new page? (default is yes)'),
    '#default_value' => variable_get('civicrm_engage_group_per_page', "1"),
  );
  return system_settings_form($form);
}

