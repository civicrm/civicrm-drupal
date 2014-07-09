<?php

namespace Drupal\civicrm_views\Plugin\views\filter;

use Drupal\views\Plugin\views\filter\InOperator;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\civicrm\Civicrm;

/**
 * @ingroup views_filter_handlers
 * @ViewsFilter("civicrm_in_operator")
 */
class CivicrmInOperator extends InOperator {
  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm')
    );
  }
}
