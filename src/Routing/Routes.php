<?php

namespace Drupal\civicrm\Routing;

use \Symfony\Component\Routing\Route;
use \Symfony\Component\Routing\RouteCollection;
use \Drupal\civicrm\CivicrmHelper;

class Routes {
  public function routes() {
    $collection = new RouteCollection();

    // Initialize CiviCRM.
    \Drupal::service('civicrm');

    $items = \CRM_Core_Menu::items();

    // CiviCRM doesn't list optional path components. So we include 5 optional components for each route,
    // and let each default to empty string.
    foreach ($items as $path => $item) {
      $route = new Route(
        '/' . $path . '/{extra}',
        array(
          '_title' => isset($item['title']) ? $item['title'] : 'CiviCRM',
          '_controller' => 'Drupal\civicrm\Controller\CivicrmController::main',
          'args' => explode('/', $path),
          'extra' => '',
        ),
        array(
          '_access' => 'TRUE',
          'extra' => '.+',
        )
      );
      $route_name = CivicrmHelper::parseURL($path)['route_name'];
      $collection->add($route_name, $route);
    }

    return $collection;
  }
}
