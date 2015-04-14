<?php

namespace Drupal\civicrm;

use Drupal\Core\DependencyInjection\ContainerInjectionInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;

class CivicrmPermissions implements ContainerInjectionInterface {
  use StringTranslationTrait;

  static function create(ContainerInterface $container) {
    return new static();
  }

  public function permissions() {
    // Initialize civicrm.
    // @Todo: Inject this via container injection instead.
    \Drupal::service('civicrm');

    $permissions = [];
    foreach (\CRM_Core_Permission::basicPermissions() as $permission => $title) {
      $permissions[$permission] = array('title' => $this->t($title));
    }
    return $permissions;
  }
}