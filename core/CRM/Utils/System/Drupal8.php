<?php
/*
 +--------------------------------------------------------------------+
 | CiviCRM version 4.4                                                |
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC (c) 2004-2013                                |
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
 *
 * @package CRM
 * @copyright CiviCRM LLC (c) 2004-2013
 * $Id$
 *
 */

/**
 * Drupal specific stuff goes here
 */
class CRM_Utils_System_Drupal8 extends CRM_Utils_System_DrupalBase {

  /**
   * Function to create a user in Drupal.
   *
   * @param array  $params associated array
   * @param string $mail email id for cms user
   *
   * @return uid if user exists, false otherwise
   *
   * @access public
   *
   */
  function createUser(&$params, $mail) {
    $user = \Drupal::currentUser();
    $user_register_conf = \Drupal::config('user.settings')->get('register');
    $verify_mail_conf = \Drupal::config('user.settings')->get('verify_mail');

    // Don't create user if we don't have permission to.
    if (!$user->hasPermission('administer users') && $user_register_conf == 'admin_only') {
      return FALSE;
    }

    $account = entity_create('user');
    $account->setUsername($params['cms_name'])->setEmail($params[$mail]);

    // Allow user to set password only if they are an admin or if
    // the site settings don't require email verification.
    if (!$verify_mail_conf || $user->hasPermission('administer users')) {
      // @Todo: do we need to check that passwords match or assume this has already been done for us?
      $account->setPassword($params['cms_pass']);
    }

    // Only activate account if we're admin or if anonymous users don't require
    // approval to create accounts.
    if ($user_register_conf != 'visitors' && !$user->hasPermission('administer users')) {
      $account->block();
    }

    // Validate the user object
    $violations = $account->validate();
    if (count($violations)) {
      return FALSE;
    }

    try {
      $account->save();
    }
    catch (\Drupal\Core\Entity\EntityStorageException $e) {
      return FALSE;
    }

    // Send off any emails as required.
    // Possible values for $op:
    //    - 'register_admin_created': Welcome message for user created by the admin.
    //    - 'register_no_approval_required': Welcome message when user
    //      self-registers.
    //    - 'register_pending_approval': Welcome message, user pending admin
    //      approval.
    // @Todo: Should we only send off emails if $params['notify'] is set?
    switch (TRUE) {
      case $user_register_conf == 'admin_only' || $user->isAuthenticated():
        _user_mail_notify('register_admin_created', $account);
        break;
      case $user_register_conf == 'visitors':
        _user_mail_notify('register_no_approval_required', $account);
        break;
      case 'visitors_admin_approval':
        _user_mail_notify('register_pending_approval', $account);
        break;
    }

    return $account->id();
  }

  /*
   *  Change user name in host CMS
   *
   *  @param integer $ufID User ID in CMS
   *  @param string $ufName User name
   */
  function updateCMSName($ufID, $ufName) {
    // CRM-5555
    if (function_exists('user_load')) {
      $user = user_load($ufID);
      // @Todo Document the reason for this conditional (inherited from Drupal 7 version)
      if ($user && $user->getEmail() != $ufName) {
        $user->setUsername($ufName);
        if (!count($user->validate())) {
          $user->save();
        }
      }
    }
  }

  /**
   * Check if username and email exists in the drupal db
   *
   * @params $params    array   array of name and mail values
   * @params $errors    array   array of errors
   * @params $emailName string  field label for the 'email'
   *
   * @return void
   */
  static function checkUserNameEmailExists(&$params, &$errors, $emailName = 'email') {
    // If we are given a name, let's check to see if it already exists.
    if (!empty($params['name'])) {
      $name = $params['name'];

      $user = entity_create('user');
      $user->setUsername($name);

      // This checks for both username uniqueness and validity.
      $violations = iterator_to_array($user->validate());
      // We only care about violations on the username field; discard the rest.
      $violations = array_filter($violations, function ($v) { return $v->getPropertyPath() == 'name.0.value'; });
      if (count($violations) > 0) {
        $errors['cms_name'] = $violations[0]->getMessage();
      }
    }

    // And if we are given an email address, let's check to see if it already exists.
    if (!empty($params[$emailName])) {
      $mail = $params[$emailName];

      $user = entity_create('user');
      $user->setEmail($mail);

      // This checks for both email uniqueness.
      $violations = iterator_to_array($user->validate());
      // We only care about violations on the email field; discard the rest.
      $violations = array_filter($violations, function ($v) { return $v->getPropertyPath() == 'mail.0.value'; });
      if (count($violations) > 0) {
        $errors[$emailName] = $violations[0]->getMessage();
      }
    }
  }

  /*
   * Function to get the drupal destination string. When this is passed in the
   * URL the user will be directed to it after filling in the drupal form
   *
   * @param object $form Form object representing the 'current' form - to which the user will be returned
   * @return string $destination destination value for URL
   *
   */
  function getLoginDestination(&$form) {
    $args = NULL;

    $id = $form->get('id');
    if ($id) {
      $args .= "&id=$id";
    }
    else {
      $gid = $form->get('gid');
      if ($gid) {
        $args .= "&gid=$gid";
      }
      else {
        // Setup Personal Campaign Page link uses pageId
        $pageId = $form->get('pageId');
        if ($pageId) {
          $component = $form->get('component');
          $args .= "&pageId=$pageId&component=$component&action=add";
        }
      }
    }

    $destination = NULL;
    if ($args) {
      // append destination so user is returned to form they came from after login
      $destination = CRM_Utils_System::currentPath() . '?reset=1' . $args;
    }
    return $destination;
  }

  /**
   * Get user login URL for hosting CMS (method declared in each CMS system class)
   *
   * @param string $destination - if present, add destination to querystring (works for Drupal only)
   *
   * @return string - loginURL for the current CMS
   * @static
   */
  public function getLoginURL($destination = '') {
    $config = CRM_Core_Config::singleton();
    $loginURL = $config->userFrameworkBaseURL;
    $loginURL .= 'user';
    if (!empty($destination)) {
      // append destination so user is returned to form they came from after login
      $loginURL .= '?destination=' . urlencode($destination);
    }
    return $loginURL;
  }


  /**
   * sets the title of the page
   *
   * @param string $title
   * @param string $pageTitle
   *
   * @return void
   * @access public
   */
  function setTitle($title, $pageTitle = NULL) {
    if (!$pageTitle) {
      $pageTitle = $title;
    }

    \Drupal::service('civicrm.page_state')->setTitle($pageTitle);
  }

  /**
   * Append an additional breadcrumb tag to the existing breadcrumb
   *
   * @param string $title
   * @param string $url
   *
   * @return void
   * @access public
   */
  function appendBreadCrumb($breadCrumbs) {
    //$breadCrumb = drupal_get_breadcrumb();

    if (is_array($breadCrumbs)) {
      foreach ($breadCrumbs as $crumbs) {
        if (stripos($crumbs['url'], 'id%%')) {
          $args = array('cid', 'mid');
          foreach ($args as $a) {
            $val = CRM_Utils_Request::retrieve($a, 'Positive', CRM_Core_DAO::$_nullObject,
              FALSE, NULL, $_GET
            );
            if ($val) {
              $crumbs['url'] = str_ireplace("%%{$a}%%", $val, $crumbs['url']);
            }
          }
        }
        $breadCrumb[] = "<a href=\"{$crumbs['url']}\">{$crumbs['title']}</a>";
      }
    }
    //drupal_set_breadcrumb($breadCrumb);
  }

  /**
   * Reset an additional breadcrumb tag to the existing breadcrumb
   *
   * @return void
   * @access public
   */
  function resetBreadCrumb() {
    $bc = array();
    drupal_set_breadcrumb($bc);
  }

  /**
   * Append a string to the head of the html file
   *
   * @param string $header the new string to be appended
   *
   * @return void
   * @access public
   */
  function addHTMLHead($header) {
    static $count = 0;
    if (!empty($header)) {
      $key = 'civi_' . ++$count;
      $data = array(
        '#type' => 'markup',
        '#markup' => $header,
      );
      drupal_add_html_head($data, $key);
    }
  }

  /**
   * Add a script file
   *
   * @param $url: string, absolute path to file
   * @param $region string, location within the document: 'html-header', 'page-header', 'page-footer'
   *
   * Note: This function is not to be called directly
   * @see CRM_Core_Region::render()
   *
   * @return bool TRUE if we support this operation in this CMS, FALSE otherwise
   * @access public
   */
  public function addScriptUrl($url, $region) {
    $options = array('group' => JS_LIBRARY, 'weight' => 10);
    switch ($region) {
      case 'html-header':
      case 'page-footer':
        $options['scope'] = substr($region, 5);
        break;
      default:
        return FALSE;
    }
    // If the path is within the drupal directory we can use the more efficient 'file' setting
    $options['type'] = $this->formatResourceUrl($url) ? 'file' : 'external';
    \Drupal::service('civicrm.page_state')->addJS($url, $options);
    return TRUE;
  }

  /**
   * Add an inline script
   *
   * @param $code: string, javascript code
   * @param $region string, location within the document: 'html-header', 'page-header', 'page-footer'
   *
   * Note: This function is not to be called directly
   * @see CRM_Core_Region::render()
   *
   * @return bool TRUE if we support this operation in this CMS, FALSE otherwise
   * @access public
   */
  public function addScript($code, $region) {
    $options = array('type' => 'inline', 'group' => JS_LIBRARY, 'weight' => 10);
    switch ($region) {
      case 'html-header':
      case 'page-footer':
        $options['scope'] = substr($region, 5);
        break;
      default:
        return FALSE;
    }
    \Drupal::service('civicrm.page_state')->addJS($code, $options);
    return TRUE;
  }

  /**
   * Add a css file
   *
   * @param $url: string, absolute path to file
   * @param $region string, location within the document: 'html-header', 'page-header', 'page-footer'
   *
   * Note: This function is not to be called directly
   * @see CRM_Core_Region::render()
   *
   * @return bool TRUE if we support this operation in this CMS, FALSE otherwise
   * @access public
   */
  public function addStyleUrl($url, $region) {
    if ($region != 'html-header') {
      return FALSE;
    }
    $options = array();
    // If the path is within the drupal directory we can use the more efficient 'file' setting
    $options['type'] = $this->formatResourceUrl($url) ? 'file' : 'external';
    \Drupal::service('civicrm.page_state')->addCSS($url, $options);
    return TRUE;
  }

  /**
   * Add an inline style
   *
   * @param $code: string, css code
   * @param $region string, location within the document: 'html-header', 'page-header', 'page-footer'
   *
   * Note: This function is not to be called directly
   * @see CRM_Core_Region::render()
   *
   * @return bool TRUE if we support this operation in this CMS, FALSE otherwise
   * @access public
   */
  public function addStyle($code, $region) {
    if ($region != 'html-header') {
      return FALSE;
    }
    $options = array('type' => 'inline');
    \Drupal::service('civicrm.page_state')->addCSS($code, $options);
    return TRUE;
  }

  /**
   * rewrite various system urls to https
   *
   * @param null
   *
   * @return void
   * @access public
   */
  function mapConfigToSSL() {
    global $base_url;
    $base_url = str_replace('http://', 'https://', $base_url);
  }

  /**
   * figure out the post url for the form
   *
   * @param mix $action the default action if one is pre-specified
   *
   * @return string the url to post the form
   * @access public
   */
  function postURL($action) {
    if (!empty($action)) {
      return $action;
    }
    return $this->url($_GET['q']);
  }

  /**
   * @param null $path
   * @param null $query
   * @param bool $absolute
   * @param null $fragment
   * @param bool $htmlize
   * @param bool $frontend
   * @param bool $forceBackend
   * @return string
   *
   * @Todo Take into account full function parameter listing.
   */
  function url($path = NULL, $query = NULL, $absolute = FALSE, $fragment = NULL, $htmlize = TRUE, $frontend = FALSE, $forceBackend = FALSE) {
    $route_name = 'civicrm.' . implode('_', explode('/', $path));
    try {
      $url = \Drupal::url($route_name);
    }
    catch (Exception $e) {
      $url = '';
    }
    return $url;
  }


  /**
   * Authenticate the user against the drupal db
   *
   * @param string $name     the user name
   * @param string $password the password for the above user name
   * @param boolean $loadCMSBootstrap load cms bootstrap?
   * @param NULL|string $realPath filename of script
   *
   * @return mixed false if no auth
   *               array(
   *  contactID, ufID, unique string ) if success
   * @access public
   */
   static function authenticate($name, $password, $loadCMSBootstrap = FALSE, $realPath = NULL) {
    require_once 'DB.php';

    $config = CRM_Core_Config::singleton();

    $dbDrupal = DB::connect($config->userFrameworkDSN);
    if (DB::isError($dbDrupal)) {
      CRM_Core_Error::fatal("Cannot connect to drupal db via $config->userFrameworkDSN, " . $dbDrupal->getMessage());
    }

    $account = $userUid = $userMail = NULL;
    if ($loadCMSBootstrap) {
      $bootStrapParams = array();
      if ($name && $password) {
        $bootStrapParams = array(
          'name' => $name,
          'pass' => $password,
        );
      }
      CRM_Utils_System::loadBootStrap($bootStrapParams, TRUE, TRUE, $realPath);

      global $user;
      if ($user) {
        $userUid = $user->uid;
        $userMail = $user->mail;
      }
    }
    else {
      // CRM-8638
      // SOAP cannot load drupal bootstrap and hence we do it the old way
      // Contact CiviSMTP folks if we run into issues with this :)
      $cmsPath = $config->userSystem->cmsRootPath($realPath);

      require_once ("$cmsPath/includes/bootstrap.inc");
      require_once ("$cmsPath/includes/password.inc");

      $strtolower = function_exists('mb_strtolower') ? 'mb_strtolower' : 'strtolower';
      $name       = $dbDrupal->escapeSimple($strtolower($name));
      $sql        = "
SELECT u.*
FROM   {$config->userFrameworkUsersTableName} u
WHERE  LOWER(u.name) = '$name'
AND    u.status = 1
";

      $query = $dbDrupal->query($sql);
      $row = $query->fetchRow(DB_FETCHMODE_ASSOC);

      if ($row) {
        $fakeDrupalAccount = drupal_anonymous_user();
        $fakeDrupalAccount->name = $name;
        $fakeDrupalAccount->pass = $row['pass'];
        $passwordCheck = user_check_password($password, $fakeDrupalAccount);
        if ($passwordCheck) {
          $userUid = $row['uid'];
          $userMail = $row['mail'];
        }
      }
    }

    if ($userUid && $userMail) {
      CRM_Core_BAO_UFMatch::synchronizeUFMatch($account, $userUid, $userMail, 'Drupal');
      $contactID = CRM_Core_BAO_UFMatch::getContactId($userUid);
      if (!$contactID) {
        return FALSE;
      }
      return array($contactID, $userUid, mt_rand());
    }
    return FALSE;
  }

  /*
   * Load user into session
   */
  function loadUser($username) {
    global $user;

    $user = user_load_by_name($username);

    if (empty($user->uid)) {
      return FALSE;
    }

    $uid = $user->uid;
    $contact_id = CRM_Core_BAO_UFMatch::getContactId($uid);

    // lets store contact id and user id in session
    $session = CRM_Core_Session::singleton();
    $session->set('ufID', $uid);
    $session->set('userID', $contact_id);
    return TRUE;
  }

  /**
   * Perform any post login activities required by the UF -
   * e.g. for drupal: records a watchdog message about the new session, saves the login timestamp,
   * calls hook_user op 'login' and generates a new session.
   *
   * @param array params
   *
   * FIXME: Document values accepted/required by $params
   */
  function userLoginFinalize($params = array()){
    user_login_finalize($params);
  }

  /**
   * Determine the native ID of the CMS user
   *
   * @param $username
   * @return int|NULL
   */
  function getUfId($username) {
    $user = user_load_by_name($username);
    if (empty($user->uid)) {
      return NULL;
    }
    return $user->uid;
  }

  /**
   * Set a message in the UF to display to a user
   *
   * @param string $message the message to set
   *
   * @access public
   */
  function setMessage($message) {
    drupal_set_message($message);
  }

  function permissionDenied() {
    drupal_access_denied();
  }

  function logout() {
    module_load_include('inc', 'user', 'user.pages');
    return user_logout();
  }

  function updateCategories() {
    // copied this from profile.module. Seems a bit inefficient, but i dont know a better way
    // CRM-3600
    cache_clear_all();
    menu_rebuild();
  }

  /**
   * Get the default location for CiviCRM blocks
   *
   * @return string
   */
  function getDefaultBlockLocation() {
    return 'sidebar_first';
  }

  /**
   * Get the locale set in the hosting CMS
   *
   * @return string  with the locale or null for none
   */
  function getUFLocale() {
    // return CiviCRM’s xx_YY locale that either matches Drupal’s Chinese locale
    // (for CRM-6281), Drupal’s xx_YY or is retrieved based on Drupal’s xx
    // sometimes for CLI based on order called, this might not be set and/or empty
    global $language;

    if (empty($language)) {
      return NULL;
    }

    if ($language->language == 'zh-hans') {
      return 'zh_CN';
    }

    if ($language->language == 'zh-hant') {
      return 'zh_TW';
    }

    if (preg_match('/^.._..$/', $language->language)) {
      return $language->language;
    }

    return CRM_Core_I18n_PseudoConstant::longForShort(substr($language->language, 0, 2));
  }

  function getVersion() {
    return defined('VERSION') ? VERSION : 'Unknown';
  }

  /**
   * load drupal bootstrap
   *
   * @param array $params Either uid, or name & pass.
   * @param boolean $loadUser boolean Require CMS user load.
   * @param boolean $throwError If true, print error on failure and exit.
   * @param boolean|string $realPath path to script
   */
  function loadBootStrap($params = array(), $loadUser = TRUE, $throwError = TRUE, $realPath = NULL) {
    //take the cms root path.
    $cmsPath = $this->cmsRootPath($realPath);

    if (!file_exists("$cmsPath/includes/bootstrap.inc")) {
      if ($throwError) {
        echo '<br />Sorry, could not locate bootstrap.inc\n';
        exit();
      }
      return FALSE;
    }
    // load drupal bootstrap
    chdir($cmsPath);
    define('DRUPAL_ROOT', $cmsPath);

    // For drupal multi-site CRM-11313
    if ($realPath && strpos($realPath, 'sites/all/modules/') === FALSE) {
      preg_match('@sites/([^/]*)/modules@s', $realPath, $matches);
      if (!empty($matches[1])) {
        $_SERVER['HTTP_HOST'] = $matches[1];
      }
    }
    require_once 'includes/bootstrap.inc';
    // @ to suppress notices eg 'DRUPALFOO already defined'.
    @drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

    // explicitly setting error reporting, since we cannot handle drupal related notices
    error_reporting(1);
    if (!function_exists('module_exists') || !module_exists('civicrm')) {
      if ($throwError) {
        echo '<br />Sorry, could not load drupal bootstrap.';
        exit();
      }
      return FALSE;
    }

    // seems like we've bootstrapped drupal
    $config = CRM_Core_Config::singleton();

    // lets also fix the clean url setting
    // CRM-6948
    $config->cleanURL = (int) variable_get('clean_url', '0');

    // we need to call the config hook again, since we now know
    // all the modules that are listening on it, does not apply
    // to J! and WP as yet
    // CRM-8655
    CRM_Utils_Hook::config($config);

    if (!$loadUser) {
      return TRUE;
    }

    $uid = CRM_Utils_Array::value('uid', $params);
    if (!$uid) {
      //load user, we need to check drupal permissions.
      $name = CRM_Utils_Array::value('name', $params, FALSE) ? $params['name'] : trim(CRM_Utils_Array::value('name', $_REQUEST));
      $pass = CRM_Utils_Array::value('pass', $params, FALSE) ? $params['pass'] : trim(CRM_Utils_Array::value('pass', $_REQUEST));

      if ($name) {
        $uid = user_authenticate($name, $pass);
        if (!$uid) {
          if ($throwError) {
            echo '<br />Sorry, unrecognized username or password.';
            exit();
          }
          return FALSE;
        }
      }
    }

    if ($uid) {
      $account = user_load($uid);
      if ($account && $account->uid) {
        global $user;
        $user = $account;
        return TRUE;
      }
    }

    if ($throwError) {
      echo '<br />Sorry, can not load CMS user account.';
      exit();
    }

    // CRM-6948: When using loadBootStrap, it's implicit that CiviCRM has already loaded its settings
    // which means that define(CIVICRM_CLEANURL) was correctly set.
    // So we correct it
    $config = CRM_Core_Config::singleton();
    $config->cleanURL = (int)variable_get('clean_url', '0');

    // CRM-8655: Drupal wasn't available during bootstrap, so hook_civicrm_config never executes
    CRM_Utils_Hook::config($config);

    return FALSE;
  }

  /**
   *
   */
  function cmsRootPath($scriptFilename = NULL) {
    $cmsRoot = $valid = NULL;

    if (!is_null($scriptFilename)) {
      $path = $scriptFilename;
    }
    else {
      $path = $_SERVER['SCRIPT_FILENAME'];
    }

    if (function_exists('drush_get_context')) {
      // drush anyway takes care of multisite install etc
      return drush_get_context('DRUSH_DRUPAL_ROOT');
    }
    // CRM-7582
    $pathVars = explode('/',
      str_replace('//', '/',
        str_replace('\\', '/', $path)
      )
    );

    //lets store first var,
    //need to get back for windows.
    $firstVar = array_shift($pathVars);

    //lets remove sript name to reduce one iteration.
    array_pop($pathVars);

    //CRM-7429 --do check for upper most 'includes' dir,
    //which would effectually work for multisite installation.
    do {
      $cmsRoot = $firstVar . '/' . implode('/', $pathVars);
      $cmsIncludePath = "$cmsRoot/includes";
      //stop as we found bootstrap.
      if (@opendir($cmsIncludePath) &&
        file_exists("$cmsIncludePath/bootstrap.inc")
      ) {
        $valid = TRUE;
        break;
      }
      //remove one directory level.
      array_pop($pathVars);
    } while (count($pathVars));

    return ($valid) ? $cmsRoot : NULL;
  }

  /**
   * check is user logged in.
   *
   * @return boolean true/false.
   */
  public function isUserLoggedIn() {
    $isloggedIn = FALSE;
    if (function_exists('user_is_logged_in')) {
      $isloggedIn = user_is_logged_in();
    }

    return $isloggedIn;
  }

  /**
   * Get currently logged in user uf id.
   *
   * @return int $userID logged in user uf id.
   */
  public function getLoggedInUfID() {
    $ufID = NULL;
    if (function_exists('user_is_logged_in') &&
      user_is_logged_in() &&
      function_exists('user_uid_optional_to_arg')
    ) {
      $ufID = user_uid_optional_to_arg(array());
    }

    return $ufID;
  }

  /**
   * Format the url as per language Negotiation.
   *
   * @param string $url
   *
   * @return string $url, formatted url.
   * @static
   */
  function languageNegotiationURL($url, $addLanguagePart = TRUE, $removeLanguagePart = FALSE) {
    if (empty($url)) {
      return $url;
    }

    //CRM-7803 -from d7 onward.
    $config = CRM_Core_Config::singleton();
    if (function_exists('variable_get') &&
      module_exists('locale') &&
      function_exists('language_negotiation_get')
    ) {
      global $language;

      //does user configuration allow language
      //support from the URL (Path prefix or domain)
      if (language_negotiation_get('language') == 'locale-url') {
        $urlType = variable_get('locale_language_negotiation_url_part');

        //url prefix
        if ($urlType == LOCALE_LANGUAGE_NEGOTIATION_URL_PREFIX) {
          if (isset($language->prefix) && $language->prefix) {
            if ($addLanguagePart) {
              $url .= $language->prefix . '/';
            }
            if ($removeLanguagePart) {
              $url = str_replace("/{$language->prefix}/", '/', $url);
            }
          }
        }
        //domain
        if ($urlType == LOCALE_LANGUAGE_NEGOTIATION_URL_DOMAIN) {
          if (isset($language->domain) && $language->domain) {
            if ($addLanguagePart) {
              $url = (CRM_Utils_System::isSSL() ? 'https' : 'http') . '://' . $language->domain . base_path();
            }
            if ($removeLanguagePart && defined('CIVICRM_UF_BASEURL')) {
              $url = str_replace('\\', '/', $url);
              $parseUrl = parse_url($url);

              //kinda hackish but not sure how to do it right
              //hope http_build_url() will help at some point.
              if (is_array($parseUrl) && !empty($parseUrl)) {
                $urlParts           = explode('/', $url);
                $hostKey            = array_search($parseUrl['host'], $urlParts);
                $ufUrlParts         = parse_url(CIVICRM_UF_BASEURL);
                $urlParts[$hostKey] = $ufUrlParts['host'];
                $url                = implode('/', $urlParts);
              }
            }
          }
        }
      }
    }

    return $url;
  }

  /**
   * Find any users/roles/security-principals with the given permission
   * and replace it with one or more permissions.
   *
   * @param $oldPerm string
   * @param $newPerms array, strings
   *
   * @return void
   */
  function replacePermission($oldPerm, $newPerms) {
    $roles = user_roles(FALSE, $oldPerm);
    if (!empty($roles)) {
      foreach (array_keys($roles) as $rid) {
        user_role_revoke_permissions($rid, array($oldPerm));
        user_role_grant_permissions($rid, $newPerms);
      }
    }
  }

  /**
   * Get a list of all installed modules, including enabled and disabled ones
   *
   * @return array CRM_Core_Module
   */
  function getModules() {
    $result = array();
    $q = db_query('SELECT name, status FROM {system} WHERE type = \'module\' AND schema_version <> -1');
    foreach ($q as $row) {
      $result[] = new CRM_Core_Module('drupal.' . $row->name, ($row->status == 1) ? TRUE : FALSE);
    }
    return $result;
  }

  /**
   * Wrapper for og_membership creation
   *
   * @param integer $ogID Organic Group ID
   * @param integer $drupalID drupal User ID
   */
  function og_membership_create($ogID, $drupalID){
    if (function_exists('og_entity_query_alter')) {
      // sort-of-randomly chose a function that only exists in the // 7.x-2.x branch
      //
      // @TODO Find more solid way to check - try system_get_info('module', 'og').
      //
      // Also, since we don't know how to get the entity type of the // group, we'll assume it's 'node'
      og_group('node', $ogID, array('entity' => user_load($drupalID)));
    }
    else {
      // Works for the OG 7.x-1.x branch
      og_group($ogID, array('entity' => user_load($drupalID)));
    }
  }

  /**
   * Wrapper for og_membership deletion
   *
   * @param integer $ogID Organic Group ID
   * @param integer $drupalID drupal User ID
   */
  function og_membership_delete($ogID, $drupalID) {
    if (function_exists('og_entity_query_alter')) {
      // sort-of-randomly chose a function that only exists in the 7.x-2.x branch
      // TODO: Find a more solid way to make this test
      // Also, since we don't know how to get the entity type of the group, we'll assume it's 'node'
      og_ungroup('node', $ogID, 'user', user_load($drupalID));
    } else {
      // Works for the OG 7.x-1.x branch
      og_ungroup($ogID, 'user', user_load($drupalID));
    }
  }

  /**
   * Get timezone from Drupal
   * @return boolean|string
   */
  function getTimeZoneOffset(){
    global $user;
    if (variable_get('configurable_timezones', 1) && $user->uid && strlen($user->timezone)) {
      $timezone = $user->timezone;
    } else {
      $timezone = variable_get('date_default_timezone', null);
    }
    $tzObj = new DateTimeZone($timezone);
    $dateTime = new DateTime("now", $tzObj);
    $tz = $tzObj->getOffset($dateTime);

    if(empty($tz)){
      return false;
    }

    $timeZoneOffset = sprintf("%02d:%02d", $tz / 3600, abs(($tz/60)%60));

    if($timeZoneOffset > 0){
      $timeZoneOffset = '+' . $timeZoneOffset;
    }
    return $timeZoneOffset;
  }
  /**
   * Reset any system caches that may be required for proper CiviCRM
   * integration.
   */
  function flush() {
    drupal_flush_all_caches();
  }
}
