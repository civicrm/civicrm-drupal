<?php

use \Drupal\Core\Session\AccountInterface;

/**
 * Implements hook_user_login().
 */
function civicrm_user_login(AccountInterface $account) {
  \Drupal::service('civicrm')->synchronizeUser($account);
}

/**
 * Implements hook_user_insert().
 */
function civicrm_user_insert(AccountInterface $account) {
  $civicrm = \Drupal::service('civicrm');
  $civicrm->synchronizeUser($account);

  // As per CRM-7858, the email address in CiviCRM isn't always set
  // with a call to synchronize(). So we force this through.
  $contact_id = \CRM_Core_BAO_UFMatch::getContactId($account->id());
  if (empty($contact_id)) {
    return;
  }
  \CRM_Core_BAO_UFMatch::updateContactEmail($contact_id, $account->getEmail());

  // Process any profile form fields that may have been submitted.
  // In particular, this will pick up form fields that were submitted on the user_registration page.
  \CRM_Core_BAO_UFGroup::getEditHTML($contact_id, '', 2, TRUE, FALSE, NULL, FALSE, $civicrm->getCtype());
}

/**
 * Implements hook_user_update().
 */
function civicrm_user_update(AccountInterface $account) {
  \Drupal::service('civicrm');

  // Update primary email address of contact if it has changed.
  $contact_id = \CRM_Core_BAO_UFMatch::getContactId($account->id());
  if ($contact_id) {
    $contact_email = \CRM_Contact_BAO_Contact::getPrimaryEmail($contact_id);
    if ($contact_email != $account->getEmail()) {
      \CRM_Core_BAO_UFMatch::updateContactEmail($contact_id, $account->getEmail());
    }
  }

  // @Todo Drupal 7 code cleared navigation menu if it detected a change in roles.
  // \CRM_Core_BAO_Navigation::resetNavigation($contact_id);
}

/**
 * Implements hook_user_delete().
 */
function civicrm_user_delete(AccountInterface $account) {
  \Drupal::service('civicrm');
  \CRM_Core_BAO_UFMatch::deleteUser($account->id());
}