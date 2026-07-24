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
    $to = str_replace(["\r", "\n"], ',', $to);
    $toEmails = explode(',', $to);
    if (empty($toEmails)) {
      return;
    }

    $tokens['mailing'] = [
      'mailing.editUrl',
      'mailing.scheduleUrl',
      'mailing.html',
      'mailing.creatorEmail',
      'mailing.creator',
    ];

    $params = [];
    $params['from'] = !empty($from) ? str_replace([
      "\r",
      "\n",
    ], '', $from) : 'Admin';

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
    $tokens['user'] = [
      'user.permission-schedule mailings',
      'user.permission-create mailings',
      'user.permission-approve mailings',
    ];
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
        $userEmails = explode(',', _civicrm_rules_replaceUserTokens($params['toEmail'], $tokens));
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

/**
 * Replace all user tokens in $str
 *
 * @param string $str
 *   The string with tokens to be replaced.
 *
 * @param null $knownTokens
 * @param bool $escapeSmarty
 *
 * @return string
 *   The processed string
 *
 */
function _civicrm_rules_replaceUserTokens($str, $knownTokens = NULL, $escapeSmarty = FALSE) {
  $key = 'user';
  if (!$knownTokens || !isset($knownTokens[$key])) {
    return $str;
  }

  $str = preg_replace_callback(
    _civicrm_rules_tokenRegex($key),
    function($matches) use ($escapeSmarty) {
      return _civicrm_rules_getUserTokenReplacement($matches[1], $escapeSmarty);
    },
    $str
  );
  return $str;
}

/**
 * @param $token
 * @param bool $escapeSmarty
 *
 * @return string
 */
function _civicrm_rules_getUserTokenReplacement($token, $escapeSmarty = FALSE) {
  $value = '';

  [$objectName, $objectValue] = explode('-', $token, 2);

  switch ($objectName) {
    case 'permission':
      $value = CRM_Core_Config::singleton()->userPermissionClass->permissionEmails($objectValue);
      break;

    case 'role':
      $value = CRM_Core_Config::singleton()->userRoleClass->roleEmails($objectValue);
      break;
  }

  if ($escapeSmarty) {
    $value = _civicrm_rules_tokenEscapeSmarty($value);
  }

  return $value;
}

/**
 * Get the regex for token replacement
 *
 * @param string $token_type
 *   A string indicating the the type of token to be used in the expression.
 *
 * @return string
 *   regular expression suitable for using in preg_replace
 */
function _civicrm_rules_tokenRegex(string $token_type) {
  return '/(?<!\{|\\\\)\{' . $token_type . '\.([\w]+(:|\.)?\w*(\-[\w\s]+)?)\}(?!\})/';
}

/**
 * Escape the string so a malicious user cannot inject smarty code into the template.
 *
 * @param string $string
 *   A string that needs to be escaped from smarty parsing.
 *
 * @return string
 *   the escaped string
 */
function _civicrm_rules_tokenEscapeSmarty($string) {
  // need to use negative look-behind, as both str_replace() and preg_replace() are sequential
  return preg_replace(['/{/', '/(?<!{ldelim)}/'], ['{ldelim}', '{rdelim}'], $string);
}
