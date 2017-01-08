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
          // discover any additional path components after the registered route
          
          if (strlen($item) > strlen($longest)) {
           $longest = $item;
          }
        }
      }
      if (!empty($longest)) {
       
        $regex_path = '|^' . str_replace('/', '\/', $longest) . '\/|';
        $params = preg_replace($regex_path, '', $path);
         
          // replace slashes with colons and the controller will piece it back together
        if(strlen($params)) {
          $params = str_replace('/', ':', $params);
        }
        $new_path = "$longest/$params";
        return $new_path;     
      }
    }
    return $path;
  }
}
