<?php

/**
 * @file
 * Contains of \Drupal\taxonomy\TermBreadcrumbBuilder.
 */

namespace Drupal\civicrm;

use Drupal\civicrm\CivicrmHelper;
use Drupal\civicrm\CivicrmPageState;
use Drupal\Core\Breadcrumb\BreadcrumbBuilderBase;
use Symfony\Cmf\Component\Routing\RouteObjectInterface;

/**
 * Provides a custom taxonomy breadcrumb builder that uses the term hierarchy.
 */
class CivicrmBreadcrumbBuilder extends BreadcrumbBuilderBase {
  protected $civicrmPageState;

  public function __construct(CivicrmPageState $civicrmPageState) {
    $this->civicrmPageState = $civicrmPageState;
  }

  /**
   * {@inheritdoc}
   */
  public function applies(array $attributes) {
    if (isset($attributes['_content']) && $attributes['_content'] == 'Drupal\civicrm\Controller\CivicrmController::main') {
      return TRUE;
    }
    return FALSE;
  }

  /**
   * {@inheritdoc}
   */
  public function build(array $attributes) {
    $breadcrumbs = array();
    $breadcrumbs[] = l(t('Home'), '<front>');

    foreach ($this->civicrmPageState->getBreadcrumbs() as $name => $url) {
      // We expect all urls to have already been passed through the url helper, and therefore
      // be valid Drupal urls.
      $breadcrumbs[] = "<a href=\"{$url}\">{$name}</a>";
    }
    return $breadcrumbs;
  }
}