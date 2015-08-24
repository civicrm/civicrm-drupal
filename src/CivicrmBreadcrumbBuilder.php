<?php

/**
 * @file
 * Contains of \Drupal\taxonomy\TermBreadcrumbBuilder.
 */

namespace Drupal\civicrm;

use Drupal\civicrm\CivicrmPageState;
use Drupal\Core\Breadcrumb\BreadcrumbBuilderInterface;
use Drupal\Core\Breadcrumb\Breadcrumb;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\Link;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\StringTranslation\TranslationInterface;
use Drupal\Core\Url;

/**
 * Provides a custom taxonomy breadcrumb builder that uses the term hierarchy.
 */
class CivicrmBreadcrumbBuilder implements BreadcrumbBuilderInterface {
  use StringTranslationTrait;

  protected $civicrmPageState;

  public function __construct(TranslationInterface $stringTranslation, CivicrmPageState $civicrmPageState) {
    $this->stringTranslation = $stringTranslation;
    $this->civicrmPageState = $civicrmPageState;
  }

  /**
   * {@inheritdoc}
   */
  public function applies(RouteMatchInterface $route_match) {
    $route_object = $route_match->getRouteObject();
    if ($route_object) {
      $controller = $route_object->getDefault('_controller');
      if (isset($controller) && $controller == 'Drupal\civicrm\Controller\CivicrmController::main') {
        return TRUE;
      }
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function build(RouteMatchInterface $route_match) {
    $breadcrumb = new Breadcrumb();
    $breadcrumb->addLink(Link::createFromRoute($this->t('Home'), '<front>'));

    foreach ($this->civicrmPageState->getBreadcrumbs() as $name => $url) {
      // Unfortunately, all urls have been passed through CRM_Utils_System::url,
      // so we need to unpack the url to construct it as a Drupal Url object.
      // Additionally, for some reason that I cannot fathom, CiviCRM is htmlentity
      // encoding the urls — so we have to decode this first.
      $url = Url::fromUserInput(html_entity_decode($url));
      $breadcrumb->addLink(new Link($name, $url));
    }
    return $breadcrumb;
  }
}