<?php

/**
 * Canned "Hey, you didn't install yet!" warning.
 */
function _civicrm_library_warning() {
  $t = get_t();
  $message[] = $t("CiviCRM module requires the installation of CiviCRM Library to function. Select a method to install it below:");
  $options[] = $t("Execute the Drush command <code>drush civi-gimmetehcodez</code> against your site.");
  $options[] = $t("Follow the !instructions_to_manually_install.", array('!instructions_to_manually_install' => l('instructions to manually install the CiviCRM library', 'http://civicrm.org/drupal7-manual-library-install')));
  $options[] = $t("Do a fancy Git dance.");
  $message[] = theme('item_list', array('items' => $options));
  $message = implode('<br />', $message);
  return $message;
}

/**
 * Check if CiviCRM library is installed.
 */
function _civicrm_library_installed() {
  $path = libraries_get_path('civicrm');
  if (module_exists('libraries') && file_exists($path)) {
    return TRUE;
  }
}

/**
 * Get CiviCRM query parameters from the url.
 *
 * This is useful for re-adding them to generated urls as drupal tends to drop
 * them and we need them for language switching and generating urls for metadata.
 */
function _civicrm_get_url_parameters() {
  $query = $_REQUEST;
  unset($query['IDS_request_uri']);
  unset($query['IDS_user_agent']);
  return $query;
}

/**
 * Get CiviCRM query parameters from the url as a string for url output.
 *
 * Drupal tends to strip CiviCRM parameters from urls and we sometimes want to put them back.
 * For example drupal will output civicrm/contribution/transact as the metadata url whereas
 * we want civicrm/contribution/transact?reset=1&id=2
 *
 * @return string
 *   String of url parameters e.g '?reset=1&id=2'.
 */
function _civicrm_get_url_parameters_as_url_string() {
  $string = '';
  foreach (_civicrm_get_url_parameters() as $key => $value) {
    if (empty($string)) {
      $string = '?' . $key . '=' . $value;
    }
    else {
      $string .= '&' . $key . '=' . $value;
    }
  }
  return $string;
}

/**
 * Find & register class loader and store location in Drupal variable.
 *
 * Per CRM-13737 this allows for drupal code to be outside the core directory
 * which makes it easier for sites managing their own installation methods that
 * may need to cover different drupal versions
 */
function _civicrm_registerClassLoader() {
  $library_path = libraries_get_path('civicrm');
  $classloader_path = variable_get('civicrm_class_loader');
  if (empty($classloader_path) || !file_exists($classloader_path)) {
    watchdog('civicrm', 'CiviCRM was unable to find classloader at %classloader_path, deleting variable', WATCHDOG_NOTICE);
    variable_del('civicrm_classloader_path');
    $candidates = array(
      'CRM/Core/ClassLoader.php',
      'civicrm-core/CRM/Core/ClassLoader.php',
      'core/CRM/Core/ClassLoader.php',
    );
    foreach ($candidates as $candidate) {
      $path = $library_path . '/' . $candidate;
      watchdog('civicrm', 'Looking for %classloader_path', array('%classloader_path' => $path), WATCHDOG_NOTICE);
      if (file_exists($path)) {
        watchdog('civicrm', 'Found %classloader_path', array('%classloader_path' => $path), WATCHDOG_NOTICE);
        require_once $path;
        CRM_Core_ClassLoader::singleton()->register();
        variable_set('civicrm_class_loader', $path);
      }
    }
  }
  else {
    require_once $classloader_path;
    CRM_Core_ClassLoader::singleton()->register();
  }
}

/**
 * Function to get the contact type.
 *
 * @param string $default contact type
 *
 * @return string $ctype
 *   Contact type
 */
function civicrm_get_ctype($default = NULL) {
  // here we are creating a new contact
  // get the contact type from the POST variables if any

  if (isset($_REQUEST['ctype'])) {
    $ctype = $_REQUEST['ctype'];
  }
  elseif (isset($_REQUEST['edit']) &&
    isset($_REQUEST['edit']['ctype'])
  ) {
    $ctype = $_REQUEST['edit']['ctype'];
  }
  else {
    $ctype = $default;
  }

  if ($ctype != 'Individual' &&
    $ctype != 'Organization' &&
    $ctype != 'Household'
  ) {
    $ctype = $default;
  }
  return $ctype;
}

/**
 * Determine if the user is on a CiviCRM generated page.
 *
 * i.e. does the form have some civicrm unique token?
 */
function civicrm_on_user_page() {
  return isset($_POST['_qf_default']);
}

function _civicrm_categories_access($profile_id) {
  if (!civicrm_initialize()) {
    return FALSE;
  }

  $allUFGroups = CRM_Core_BAO_UFGroup::getModuleUFGroup('User Account', 0, TRUE);

  if (is_array(CRM_Utils_Array::value($profile_id, $allUFGroups))) {
    return TRUE;
  }
}

/**
 * Translating profile menu title dynamicaly to overide caching
 */
function civicrm_menu_alter(&$items) {
  if (_civicrm_library_installed()) {
    $categories = civicrm_user_categories();
    foreach ($categories as $cat) {
      $path = 'user/%user_category/edit/' . $cat['name'];
      $items[$path]['title callback'] = 'civicrm_profile_title_callback';
      $items[$path]['title arguments'] = array((string) $cat['id'], $cat['title']);
    }
  }
}

function civicrm_profile_title_callback($profile_id, $fallback) {
  if (!civicrm_initialize() || empty($profile_id)) {
    return $fallback;
  }

  return CRM_Core_BAO_UFGroup::getTitle($profile_id);
}

/**
 * Get appropriate profile ID for edit screen.
 *
 * @param $name
 *
 * @return int
 *   Profile ID.
 *
 * @throws \CRM_Core_Exception
 */
function _civicrm_get_user_profile_id($name) {
  $profiles = civicrm_api3('uf_group', 'get', array(
    'name' => $name,
    'is_active' => 1,
  ));

  if (!$profiles['count']) {
    $profiles = civicrm_api3('uf_group', 'get', array(
      'title' => $name,
      'is_active' => 1,
      'options' => array('limit' => 1,),
    ));
  }

  if (!$profiles['count']) {
    // @todo I suspect the profile listing is not possible in both scenarios but need to understand what it
    // means before removing from one warning.
    throw new CRM_Core_Exception(ts('The requested Profile (%1) is disabled OR it is not configured to be used for \'Profile\' listings in its Settings OR there is no Profile with that ID. Please contact the site administrator if you need assistance.',
      array(1 => $name)
    ));
  }

  // CRM-15952 we expected 'name' to be a unique field but the DB doesn't enforce that.
  // Extra check if it is not unique here.
  if ($profiles['count'] > 1) {
    foreach ($profiles['values'] as $profile) {
      try {
        $profiles['id'] = civicrm_api3('uf_join', 'getvalue', array(
          'uf_group_id' => $profile['id'],
          'module' => 'User Account',
          'return' => 'uf_group_id',
        ));
        continue;
      }
      catch (Exception $e){
      }
    }
  }

  $ufGroupIDs = CRM_Core_Permission::ufGroupClause(CRM_Core_Permission::EDIT, NULL, TRUE);
  $profileID = $profiles['id'];
  if (!in_array($profileID, $ufGroupIDs)) {
    throw new CRM_Core_Exception(ts('The requested Profile (id = %1) is not configured to be used for \'Profile\' listings in its Settings OR there is no Profile with that ID OR you do not have permission to access this profile. Please contact the site administrator if you need assistance.',
      array(1 => $profileID)
    ));
  }
  return $profileID;
}

/**
 * Disable the drupal cache for all civicrm pages which should not be cached.
 */
function _civicrm_disable_drupal_cache() {
  if (function_exists('drupal_page_is_cacheable')) {
    // This is a Drupal 7 function only - using 'easy option' of checking function.
    drupal_page_is_cacheable(FALSE);
  }
}

/**
 * Disable civicrm key for all forms that interact with the CMS.
 *
 * We do not control the CMS form generation and hence should suppress
 * qfKey
 */
function civicrm_key_disable() {
  if (!civicrm_initialize()) {
    return FALSE;
  }

  CRM_Core_Config::singleton()->keyDisable = TRUE;
}

/**
 * Adds CiviCRM CSS and JS resources into the header.
 *
 * @TODO Rename to _civicrm_html_head() to indicate
 * that this is not a hook implementation.
 */
function _civicrm_html_head() {
  if (civicrm_initialize()) {
    if (arg(0) == 'civicrm') {
      // Add our standard css & js
      CRM_Core_Resources::singleton()->addCoreResources();
    }
    else {
      CRM_Core_Resources::singleton()->addCoreStyles();
    }
  }
}
