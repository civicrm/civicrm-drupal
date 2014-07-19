<?php

namespace Drupal\civicrm;

use Drupal\Core\Config\ConfigException;
use Drupal\Core\Session\AccountInterface;

class Civicrm {
  /**
   * Initialize CiviCRM. Call this function from other modules too if
   * they use the CiviCRM API.
   */
  public function __construct() {
    // Get ready for problems
    $docLinkInstall = "http://wiki.civicrm.org/confluence/display/CRMDOC/Drupal+Installation+Guide";
    $docLinkTrouble = "http://wiki.civicrm.org/confluence/display/CRMDOC/Installation+and+Configuration+Trouble-shooting";
    $forumLink      = "http://forum.civicrm.org/index.php/board,6.0.html";

    $errorMsgAdd = t("Please review the <a href='!1'>Drupal Installation Guide</a> and the <a href='!2'>Trouble-shooting page</a> for assistance. If you still need help installing, you can often find solutions to your issue by searching for the error message in the <a href='!3'>installation support section of the community forum</a>.</strong></p>",
      array('!1' => $docLinkInstall, '!2' => $docLinkTrouble, '!3' => $forumLink)
    );

    $settingsFile = conf_path() . '/civicrm.settings.php';
    if (!defined('CIVICRM_SETTINGS_PATH')) {
      define('CIVICRM_SETTINGS_PATH', $settingsFile);
    }

    $output = include_once $settingsFile;
    if ($output == FALSE) {
      $msg = t("The CiviCRM settings file (civicrm.settings.php) was not found in the expected location ") .
        "(" . $settingsFile . "). " . $errorMsgAdd;
      throw new ConfigException($msg);
    }

    // This does pretty much all of the civicrm initialization
    $output = include_once 'CRM/Core/Config.php';
    if ($output == FALSE) {
      $msg = t("The path for including CiviCRM code files is not set properly. Most likely there is an error in the <em>civicrm_root</em> setting in your CiviCRM settings file (!1).",
          array('!1' => $settingsFile)
        ) . t("civicrm_root is currently set to: <em>!1</em>.", array('!1' => $civicrm_root)) . $errorMsgAdd;
      throw new ConfigException($msg);
    }

    // Initialize the system by creating a config object
    \CRM_Core_Config::singleton();

    // Add module-specific header elements
    // $header = civicrm_html_head();
    // if (!empty($header)) {
    //   drupal_add_html_head($header);
    // }

    // $args = explode('/', $_GET['q']);
    // if (
    //   (!isset($args[1]) || $args[1] != 'upgrade') &&
    //   // drupal wysiwyg
    //   CRM_Core_BAO_Setting::getItem(CRM_Core_BAO_Setting::SYSTEM_PREFERENCES_NAME, 'editor_id') == 4
    // ) {
    //   // CRM-12164
    //   if (!function_exists('wysiwyg_get_profile')) {
    //     // reset the editor choice so the warning is not displayed all the time
    //     CRM_Core_BAO_Setting::setItem(NULL, CRM_Core_BAO_Setting::SYSTEM_PREFERENCES_NAME, 'editor_id');

    //     // display warning and url to fix
    //     $url = CRM_Utils_System::url('civicrm/admin/setting/preferences/display', 'reset=1');
    //     $msg = ts(
    //       'You had configured CiviCRM to use Drupal default editor but the WYSIWYG module is disabled. Please goto <a href="%1">display preferences</a> and choose another editor',
    //       array(1 => $url)
    //     );
    //     CRM_Core_Session::setStatus($msg);
    //   }
    //   else {
    //     //we have to ensure that wysiwyg is loaded since
    //     //pages load only with ajax callbacks
    //     $formats = filter_formats();
    //     foreach ($formats as $id => $format) {
    //       wysiwyg_get_profile($id);
    //     }
    //     $path = drupal_get_path('module', 'filter');
    //     drupal_add_js($path . '/filter.js');
    //     drupal_add_css($path . '/filter.css');
    //   }
    // }
    // CRM_Core_Config::singleton()->userSystem->setMySQLTimeZone();
  }

  public function invoke($args) {
    // Civicrm will echo/print directly to stdout. We need to capture it
    // so that we can return the output as a renderable array.
    ob_start();
    $content = \CRM_Core_Invoke::invoke($args);
    $output = ob_get_clean();
    return !empty($content) ? $content : $output;
  }

  /**
   * Get the civicrm admin menu.
   *
   * @return array
   */
  public function navigationTree() {
    $navigationTree = array();
    return \CRM_Core_BAO_Navigation::buildNavigationTree($navigationTree, 0);
  }

  /**
   * Synchronize a Drupal account with CiviCRM. This is a wrapper for CRM_Core_BAO_UFMatch::synchronize().
   *
   * @param AccountInterface $account
   * @param string $contact_type
   */
  public function synchronizeUser(AccountInterface $account, $contact_type = 'Individual') {
    // CiviCRM is probing Drupal user object based on the CMS type, and for Drupal it is expecting a Drupal 6/7 user object.
    // It really should be using an standardised interface and requiring the CMS's to offer an implementation.
    // Alas, we'll mock an object for it to use.
    $user = new \stdClass();
    $user->uid = $account->id();
    $user->name = $account->getUsername();
    $user->mail = $account->getEmail();
    \CRM_Core_BAO_UFMatch::synchronize($user, FALSE, 'Drupal', $this->getCtype($contact_type));
  }

  /**
   * Function to get the contact type
   *
   * @param string $default Default contact type
   *
   * @return $ctype Contact type
   *
   * @Todo: Document what this function is doing and why.
   */
  public function getCtype($default = 'Individual') {
    if (!empty($_REQUEST['ctype'])) {
      $ctype = $_REQUEST['ctype'];
    }
    else if (!empty($_REQUEST['edit']['ctype'])) {
      $ctype = $_REQUEST['edit']['ctype'];
    }
    else {
      $ctype = $default;
    }

    if (!in_array($ctype, array('Individual', 'Organization', 'Household'))) {
      $ctype = $default;
    }
    return $ctype;
  }
}