<?php

namespace Drupal\civicrm\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\civicrm\Civicrm;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides the core Civicrm blocks.
 *
 * @Block(
 *   id = "civicrm_block",
 *   admin_label = @Translation("Civicrm block"),
 *   category = @Translation("Civicrm"),
 *   deriver = "Drupal\civicrm\Plugin\Derivative\CivicrmBlock",
 * )
 */
class CivicrmBlock extends BlockBase implements ContainerFactoryPluginInterface {
  public function __construct(Civicrm $civicrm, array $configuration, $plugin_id, array $plugin_definition) {
    // We don't do anything with the Civicrm service, only ensure that it
    // has been initialized.
    parent::__construct($configuration, $plugin_id, $plugin_definition);
  }

  static public function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $container->get('civicrm'),
      $configuration,
      $plugin_id,
      $plugin_definition
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $block_id = $this->getDerivativeId();
    $content = \CRM_Core_Block::getContent($block_id)['content'];

    if ($content) {
      return array(
        '#markup' => $content,
      );
    }
    return array();
  }
}