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
      $longest = '';
      foreach (array_keys($items) as $item) {
        $item = '/' . $item;
        // if he current path is a civicrm path
        if ((strpos($path, $item ) === 0))  {
          // discover longest matching civicrm path in the request path 
          
          if (strlen($item) > strlen($longest)) {
           $longest = $item;
          }
        }
      }
      if (!empty($longest)) {
        // parse url component parameters from path
        $params = str_replace($longest, '', $path);
        // replace slashes with colons and the controller will piece it back together
        if (strlen($params)) {
          $params = str_replace('/', ':', $params);
          if (substr($params, 0, 1) == ':') {
           $params = substr($params, 1);
          }
          return "$longest/$params";
        }
        else {
          return $longest;
        }
      }
    }
    return $path;
  }
}
