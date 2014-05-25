<?php

namespace Drupal\civicrm\Plugin\Derivative;

use Drupal\civicrm\Civicrm;
use Drupal\Component\Plugin\Derivative\DerivativeBase;
use Drupal\Core\Plugin\Discovery\ContainerDerivativeInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class LocalTasks extends DerivativeBase implements ContainerDerivativeInterface {
  public function __construct(Civicrm $civicrm) {
    // We don't do anything with the Civicrm service, only ensure that it
    // has been initialized.
  }

  static public function create(ContainerInterface $container, $base_plugin_id) {
    return new static(
      $container->get('civicrm')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    $uf_groups = \CRM_Core_BAO_UFGroup::getModuleUFGroup('User Account');

    foreach ($uf_groups as $key => $uf_group) {
      if ($uf_group['is_active']) {
        $this->derivatives["civicrm.{$key}"] = $base_plugin_definition;
        $this->derivatives["civicrm.{$key}"]['title'] = $uf_group['title'];
        $this->derivatives["civicrm.{$key}"]['route_parameters'] = array('profile' => $key);
      }
    }

    return $this->derivatives;
  }
}