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

  public function main($args) {
    $content = $this->civicrm->invoke($args);

    if ($this->civicrmPageState->isAccessDenied()) {
      throw new \Symfony\Component\HttpKernel\Exception\AccessDeniedHttpException();
    }

    // Synchronize the Drupal user with the Contacts database (why?)
    // @Todo: reimplement civicrm_get_ctype()?
    // CiviCRM is probing Drupal user object based on the CMS type, and for Drupal it is expecting a Drupal 6/7 user object.
    // It really should be using an standardised interface and requiring the CMS's to offer an implementation.
    // Alas, we'll mock an object for it to use.
    $account = new \stdClass();
    $account->uid = $this->currentUser()->id();
    $account->name = $this->currentUser()->getUsername();
    $account->mail = $this->currentUser()->getEmail();
    \CRM_Core_BAO_UFMatch::synchronize($account, FALSE, 'Drupal', 'Individual');

    // Add CSS, JS, etc. that is required for this page.
    \CRM_Core_Resources::singleton()->addCoreResources();
    if ($region = \CRM_Core_Region::instance('html-header', FALSE)) {
      \CRM_Utils_System::addHTMLHead($region->render(''));
    }

    $build = array(
      '#attached' => array(
        'css' => $this->civicrmPageState->getCSS(),
        'js' => $this->civicrmPageState->getJS(),
      ),
      '#markup' => $content,
    );

    // Override default title value if one has been set in the course
    // of calling \CRM_Core_Invoke::invoke().
    if ($title = $this->civicrmPageState->getTitle()) {
      $build['#title'] = $title;
    }

    return $build;
  }
}
