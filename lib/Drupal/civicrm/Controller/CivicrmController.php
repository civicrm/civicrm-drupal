<?php

/**
 * @file
 * Contains \Drupal\civicrm\Controller\CivicrmController
 */

namespace Drupal\civicrm\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\Request;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\civicrm\CivicrmPageState;
use Drupal\civicrm\Civicrm;

class CivicrmController extends ControllerBase {
  protected $civicrmPageState;
  protected $request;
  protected $civicrm;

  public function __construct(Request $request, CivicrmPageState $civicrmPageState, Civicrm $civicrm) {
    $this->request = $request;
    $this->civicrmPageState = $civicrmPageState;
    $this->civicrm = $civicrm;
  }

  public static function create(ContainerInterface $container) {
    return new static(
      $container->get('request'),
      $container->get('civicrm.page_state'),
      $container->get('civicrm')
    );
  }

  public function main() {
    $content = $this->civicrm->invoke($this->request->attributes->get('args', array('civicrm')));

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
