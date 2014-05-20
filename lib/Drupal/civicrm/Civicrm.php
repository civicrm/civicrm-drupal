<?php

namespace Drupal\civicrm;

use Drupal\Core\Config\ConfigException;

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

    // Check for php version and ensure its greater than minPhpVersion
    // $minPhpVersion = '5.3.3';
    // if (version_compare(PHP_VERSION, $minPhpVersion) < 0) {
    //   echo "CiviCRM requires PHP Version $minPhpVersion or greater. You are running PHP Version " . PHP_VERSION . "<p>";
    //   exit();
    // }
    $this->registerClassLoader();

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
    \CRM_Core_BAO_Navigation::buildNavigationTree($navigationTree, 0);

    // Separate path and query string in URL component
    foreach ($navigationTree as &$leaf) {
      $this->_NavigationTreeURLFix($leaf);
    }
    return $navigationTree;
  }

  /**
   * Recursive function helper to separate navigation URLs into
   * their paths and query parameters.
   *
   * @param $leaf
   */
  protected function _NavigationTreeURLFix(&$leaf) {
    // Recurse on child menu items.
    if (is_array($leaf['child'])) {
      foreach ($leaf['child'] as &$child) {
        $this->_NavigationTreeURLFix($child);
      }
    }

    // Separate out the url into its path and query components.
    $url = parse_url($leaf['attributes']['url']);
    if (!empty($url['path'])) {
      $leaf['attributes']['url'] = $url['path'];
    }

    // Turn the query string (if it exists) into an associative array.
    $query = array();
    if (!empty($url['query'])) {
      parse_str($url['query'], $query);
    }
    $leaf['attributes']['query'] = $query;
  }

  /**
   * Find & register classloader and store location in Drupal variable.
   * Per CRM-13737 this allows for drupal code to be outside the core directory
   * which makes it easier for sites managing their own installation methods that
   * may need to cover different drupal versions
   */
  protected function registerClassLoader() {
    //$path = variable_get('civicrm_class_loader');
    if (empty($path) || !file_exists($path)) {
      $candidates = array(
        dirname(__FILE__) . '/../../../../CRM/Core/ClassLoader.php',
        dirname(__FILE__) . '/../../../../civicrm-core/CRM/Core/ClassLoader.php',
        dirname(__FILE__) . '/../../../../core/CRM/Core/ClassLoader.php',
        // ... ad nauseum ...
      );

      foreach ($candidates as $candidate) {
        if (file_exists($candidate)) {
          $path = $candidate;
          //variable_set('civicrm_class_loader', $candidate);
          break;
        }
      }
    }

    require_once $path;
    \CRM_Core_ClassLoader::singleton()->register();
  }
}