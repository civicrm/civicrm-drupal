<?php

namespace Drupal\civicrm_views\Plugin\views\relationship;

use Drupal\views\Plugin\views\relationship\RelationshipPluginBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\views\ViewExecutable;
use Drupal\views\Plugin\views\display\DisplayPluginBase;
use Drupal\civicrm\Civicrm;
use Drupal\core\form\FormStateInterface;

/**
 * @ingroup views_relationship_handlers
 * @ViewsRelationship("civicrm_location")
 */
class CivicrmLocation extends RelationshipPluginBase {
  protected $locations = array();
  protected $default_location = NULL;

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->default_location = \CRM_Core_BAO_LocationType::getDefault()->id;
    $this->locations = \CRM_Core_BAO_Address::buildOptions('location_type_id');

  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm')
    );
  }

  public function init(ViewExecutable $view, DisplayPluginBase $display, array &$options = NULL) {
    parent::init($view, $display, $options);

    $this->definition['extra'] = array();
    if (!empty($this->options['location_type'])) {
      $this->definition['extra'][] = array(
        'field' => 'location_type_id',
        'value' => (int) ($this->options['location_type'] == 'default' ? $this->default_location : $this->options['location_type']),
        'numeric' => TRUE,
      );
    }
    if (!empty($this->options['is_primary'])) {
      $this->definition['extra'][] = array(
        'field' => 'is_primary',
        'value' => $this->options['is_primary'],
        'numeric' => TRUE,
      );
    }
    if (!empty($this->options['is_billing'])) {
      $this->definition['extra'][] = array(
        'field' => 'is_billing',
        'value' => $this->options['is_billing'],
        'numeric' => TRUE,
      );
    }
  }

  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['location_type'] = array('default' => 0);
    $options['is_billing'] = array('default' => FALSE, 'bool' => TRUE);
    $options['is_primary'] = array('default' => FALSE, 'bool' => TRUE);
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    $form['is_primary'] = array(
      '#type' => 'checkbox',
      '#title' => t('Is primary?'),
      '#default_value' => isset($this->options['is_primary']) ? $this->options['is_primary'] : FALSE,
    );
    $form['is_billing'] = array(
      '#type' => 'checkbox',
      '#title' => t('Is billing?'),
      '#default_value' => isset($this->options['is_billing']) ? $this->options['is_billing'] : FALSE,
    );
    $form['location_type'] = array(
      '#type' => 'radios',
      '#title' => t('Location type'),
      '#options' => array(
        0 => t('Any'),
        'default' => t('Default location (!default)', array('!default' => $this->locations[$this->default_location])),
      ),
      '#default_value' => isset($this->options['location_type']) ? (int) $this->options['location_type'] : 0,
    );

    foreach ($this->locations as $id => $location) {
      $form['location_type']['#options'][$id] = $location;
    }

    parent::buildOptionsForm($form, $form_state);
  }
}