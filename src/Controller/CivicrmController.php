<?php

/**
 * @file
 * Contains \Drupal\civicrm\Controller\CivicrmController
 */

namespace Drupal\civicrm\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\civicrm\CivicrmPageState;
use Drupal\civicrm\Civicrm;
use Drupal\Component\Utility\SafeMarkup;

class CivicrmController extends ControllerBase {
  protected $civicrm;
  protected $civicrmPageState;

  public function __construct(Civicrm $civicrm, CivicrmPageState $civicrmPageState) {
    $this->civicrm = $civicrm;
    $this->civicrmPageState = $civicrmPageState;
  }

  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('civicrm'),
      $container->get('civicrm.page_state')
    );
  }

  public function main($args, $extra) {
    if ($extra) {
      $args = array_merge($args, explode(':', $extra));
    }

    // CiviCRM's Invoke.php has hardwired in the expectation that the query parameter 'q' is being used.
    // We recreate that parameter. Ideally in the future, this data should be passed in explicitly and not tied
    // to an environment variable.
    $_GET['q'] = implode('/', $args);

    // @Todo: Enable CiviCRM's CRM_Core_TemporaryErrorScope::useException() and possibly catch exceptions.
    // At the moment, civicrm doesn't allow exceptions to bubble up to Drupal. See CRM-15022.
    $content = $this->civicrm->invoke($args);

    if ($this->civicrmPageState->isAccessDenied()) {
      throw new \Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException();
    }

    // Synchronize the Drupal user with the Contacts database (why?)
    $this->civicrm->synchronizeUser(\Drupal\user\Entity\User::load($this->currentUser()->id()));

    // Add CSS, JS, etc. that is required for this page.
    \CRM_Core_Resources::singleton()->addCoreResources();
    if ($region = \CRM_Core_Region::instance('html-header', FALSE)) {
      \CRM_Utils_System::addHTMLHead($region->render(''));
    }

    // We set the CiviCRM markup as safe and assume all XSS (an other) issues have already
    // been taken care of. The SafeMarkup::set() function is stated to be used for
    // internal use only, so this is a cludge.
    $build = array(
      '#markup' => SafeMarkup::format($content, []),
    );
    $counter = 0;
    foreach ($this->civicrmPageState->getCSS() as $css) {
      $build['#attached']['html_head'][] = array($css, 'civicrm-controller-' . $counter);
      $counter++;
    }
    foreach ($this->civicrmPageState->getJS() as $js) {
      $build['#attached']['html_head'][] = array($js, 'civicrm-controller-' . $counter);
      $counter++;
    }

    // Override default title value if one has been set in the course
    // of calling \CRM_Core_Invoke::invoke().
    if ($title = $this->civicrmPageState->getTitle()) {
      // Mark the pageTitle as safe so markup is not escaped by Drupal.
      // This handles the case where, eg. the page title is surrounded by <span id="crm-remove-title" style=display: none">
      // Todo: This is a naughty way to do this. Better to have CiviCRM passing us no markup whatsoever.
      \Drupal\Component\Utility\SafeMarkup::format($title, []);
      $build['#title'] = $title;
    }

    return $build;
  }

}
