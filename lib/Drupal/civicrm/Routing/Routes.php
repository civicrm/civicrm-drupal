<?php

namespace Drupal\civicrm\Routing;

use \Symfony\Component\Routing\Route;
use \Symfony\Component\Routing\RouteCollection;

class Routes {
  public function routes() {
    $collection = new RouteCollection();

    // Initialize CiviCRM.
    \Drupal::service('civicrm');

    $items = \CRM_Core_Menu::items();

    foreach ($items as $path => $item) {
      $route = new Route(
        '/' . $path,
        array(
          '_title' => isset($item['title']) ? $item['title'] : 'CiviCRM',
          '_content' => 'Drupal\civicrm\Controller\CivicrmController::main',
          // We explicitly provide a _controller key so that this page will
          // be accessible when Accept headers are non-html.
          '_controller' => 'controller.page:content',
          'args' => explode('/', $path),
        ),
        array(
          '_access' => 'TRUE',
        )
      );

      // Create a route name by replacing the forward slashes in the path
      // with underscores, eg. civicrm/contact/search => civicrm.civicrm_contact_search
      $route_name = 'civicrm.' . implode('_', explode('/', $path));
      $collection->add($route_name, $route);
    }

    return $collection;
  }
}
