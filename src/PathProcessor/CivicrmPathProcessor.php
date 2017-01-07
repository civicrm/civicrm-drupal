<?php

namespace Drupal\civicrm\PathProcessor;

use Drupal\Core\PathProcessor\InboundPathProcessorInterface;
use Symfony\Component\HttpFoundation\Request;

class CivicrmPathProcessor implements InboundPathProcessorInterface {

  public function processInbound($path, Request $request) {
    if (strpos($path, '/civicrm/ajax/menujs') === 0) {
      $params = preg_replace('|^\/civicrm\/ajax\/menujs\/|', '', $path);
      $params = str_replace('/',':', $params);
      return "/civicrm/ajax/menujs/$params";
    }
    return $path;
  }

}
