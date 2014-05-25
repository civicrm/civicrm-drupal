<?php

namespace Drupal\civicrm\Plugin\Derivative;

use Drupal\civicrm\Civicrm;
use Drupal\Component\Plugin\Derivative\DerivativeBase;
use Drupal\Core\Plugin\Discovery\ContainerDerivativeInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

class CivicrmBlock extends DerivativeBase implements ContainerDerivativeInterface {
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
    $blocks = \CRM_Core_Block::getInfo();
    foreach ($blocks as $block_id => $block) {
      // There's no need to prefix each block label with 'CiviCRM', as in Drupal 8
      // we're already grouping our blocks in the Civicrm category.
      $label = str_replace('CiviCRM ', '', $block['info']);

      $this->derivatives[$block_id] = $base_plugin_definition;
      $this->derivatives[$block_id]['admin_label'] = $label;
      $this->derivatives[$block_id]['cache'] = DRUPAL_NO_CACHE;
    }
    return $this->derivatives;
  }
}