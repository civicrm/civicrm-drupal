<?php

namespace Drupal\civicrm\PathProcessor;

use Drupal\Core\PathProcessor\InboundPathProcessorInterface;
use Symfony\Component\HttpFoundation\Request;

use Drupal\civicrm\Civicrm;

class CivicrmPathProcessor implements InboundPathProcessorInterface {

  public function processInbound($path, Request $request) {
    // if the path is a civicrm path   
    if (strpos($path, '/civicrm/') === 0) {
      // initialize civicrm
      $civicrm = new Civicrm();
      // fetch civicrm menu items
      $items = \CRM_Core_Menu::items();
      foreach (array_keys($items) as $item) {
        // if he current path is a civicrm path
        if(('/' . $item  . '/') == $path) {
          // discover any additional path components after the registered route
          $regex_path = str_replace('/', '\/', $path);
          $params = preg_replace("|^$regex_path|", '', $path);
          // replace slashes with colons and the controller will piece it back together
          $params = str_replace('/', ':', $params);
          return "$path/$params";
        }
      }
    }
    return $path;
  }
}
