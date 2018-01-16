<?php

/**
 * @file
 * Contains \Drupal\civicrm\Controller\CivicrmController
 */

namespace Drupal\civicrm\Controller;

use Drupal\Core\Controller\ControllerBase;
use Drupal\Core\Render\Markup;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\civicrm\CivicrmPageState;
use Drupal\civicrm\Civicrm;

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

    // Need to disable the page cache.
    \Drupal::service('page_cache_kill_switch')->trigger();

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

    // We set the CiviCRM markup as safe and assume all XSS (an other) issues have already
    // been taken care of.
    $build = array(
      '#markup' => Markup::create($content),
      '#cache' => [
        'max-age' => 0,
      ],
    );

    // Override default title value if one has been set in the course
    // of calling \CRM_Core_Invoke::invoke().
    if ($title = $this->civicrmPageState->getTitle()) {
      // Mark the pageTitle as safe so markup is not escaped by Drupal.
      // This handles the case where, eg. the page title is surrounded by <span id="crm-remove-title" style=display: none">
      // Todo: This is a naughty way to do this. Better to have CiviCRM passing us no markup whatsoever.
      $build['#title'] = Markup::create($title);
    }

    return $build;
  }

}
