<?php

/**
 * (Page callback)
 *
 * @return string
 */
function civicrm_setup_page() {
  $coreUrl = dirname(file_create_url(drupal_get_path('module', 'civicrm')));
  $corePath = dirname(__DIR__);
  $classLoader = implode(DIRECTORY_SEPARATOR, [$corePath, 'CRM', 'Core', 'ClassLoader.php']);

  if (file_exists($classLoader)) {
    require_once $classLoader;
    CRM_Core_ClassLoader::singleton()->register();
    \Civi\Setup::assertProtocolCompatibility(1.0);
    \Civi\Setup::init([
      // This is just enough information to get going. Drupal.civi-setup.php does more scanning.
      'cms' => 'Drupal',
      'srcPath' => $corePath,
    ]);
    $ctrl = \Civi\Setup::instance()->createController()->getCtrl();
    $ctrl->setUrls(array(
      'ctrl' => url('civicrm'),
      'res' => $coreUrl . '/setup/res/',
      'jquery.js' => $coreUrl . '/bower_components/jquery/dist/jquery.min.js',
      'font-awesome.css' => $coreUrl . '/bower_components/font-awesome/css/all.min.css',
      // Not used? 'finished' => url('civicrm/dashboard', ['query' => ['reset' => 1],]),
    ));
    \Civi\Setup\BasicRunner::run($ctrl);
    exit();
  }
  else {
    drupal_set_message(t('Cannot perform setup for CiviCRM. The file "@file" is missing.', [
      '@file' => $classLoader,
    ]), 'error');
    return '';
  }
}

///**
// * This is a draft alternative to BasicRunner. It would theoretically enable
// * Drupal's chrome. However, the styling is a little wonky, and you wind up
// * with two <HTML>s. Maybe worth fixing someday - but not now.
// *
// * @param \Civi\Setup\UI\SetupControllerInterface $ctrl
// * @return mixed
// */
//function _civicrm_setup_runCtrl($ctrl) {
//  $method = $_SERVER['REQUEST_METHOD'];
//  list ($headers, $body) = $ctrl->run($method, ($method === 'GET' ? $_GET : $_POST));
//  foreach ($headers as $k => $v) {
//    drupal_add_http_header($k, $v, TRUE);
//  }
//  return $body;
//}
