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
 *
 * @package CRM
 * @copyright CiviCRM LLC https://civicrm.org/licensing
 */

use Civi\Token\TokenProcessor;

require_once 'civicrm_rules_utils.inc';

function civicrm_rules_rules_action_mailing_send_email($to,
  $subject,
  $message,
  $from,
  $settings,
  RulesState $state,
  RulesPlugin $element
) {
  if (isset($state->variables['mailing']) && $state->variables['mailing']->id) {
    $to = str_replace(array("\r", "\n"), ',', $to);
    $toEmails = explode(',', $to);
    if (empty($toEmails)) {
      return;
    }

    $tokens['mailing'] = array(
      'mailing.editUrl',
      'mailing.scheduleUrl',
      'mailing.html',
      'mailing.creatorEmail',
      'mailing.creator',
    );

    $params            = array();
    $params['from']    = !empty($from) ? str_replace(array(
      "\r", "\n",
    ), '', $from) : 'Admin';

    $mailingContext = $state->variables['mailing']->id ? ['mailingId' => $state->variables['mailing']->id] : [];
    $tokenProcessor = new TokenProcessor(\Civi::dispatcher(), $mailingContext + [
      'controller' => __CLASS__,
      'smarty' => FALSE,
    ]);

    $tokenProcessor->addMessage('subject', $subject, 'text/plain');
    $tokenProcessor->addMessage('html', $subject, 'text/html');
    $tokenProcessor->addRow();
    $tokenProcessor->evaluate();
    $params['subject'] = $tokenProcessor->getRow(0)->render('subject');
    $params['html'] = $tokenProcessor->getRow(0)->render('html');

    // also allow some user permission tokens for the toEmail
    $tokens['user'] = array(
      'user.permission-schedule mailings',
      'user.permission-create mailings',
      'user.permission-approve mailings',
    );
    foreach ($toEmails as $toEmail) {
      $tokenProcessor = new TokenProcessor(\Civi::dispatcher(), $mailingContext + [
        'controller' => __CLASS__,
        'smarty' => FALSE,
      ]);

      $tokenProcessor->addMessage('toEmail', $toEmail, 'text/plain');
      $tokenProcessor->addRow();
      $tokenProcessor->evaluate();
      $params['toEmail'] = $tokenProcessor->getRow(0)->render('toEmail');

      //only process user permission tokens if used in toEmail
      if (in_array(trim($toEmail, '{}'), $tokens['user'])) {
        //we require a TO email, but can only send one; so we strip the first off and send the rest as CCs
        $userEmails = explode(',', CRM_Utils_Token::replaceUserTokens($params['toEmail'], $tokens));
        $params['toEmail'] = $userEmails[0];
        array_shift($userEmails);
        $params['cc'] = implode(',', $userEmails);
      }

      CRM_Utils_Mail::send($params);
    }
  }
}

function civicrm_rules_condition_mailing_approved($mailing, $settings) {
  return $mailing->approval_status_id == 1;
}

function civicrm_rules_condition_mailing_rejected($mailing, $settings) {
  return $mailing->approval_status_id != 1;
}
