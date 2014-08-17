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
 * @ViewsRelationship("civicrm_relationship")
 */
class CivicrmRelationship extends RelationshipPluginBase {
  protected $relationships = array();

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    // relationshipType() returns information about relations as array with fields
    // 'name_a_b', 'name_b_a', 'contact_type_a' and 'contact_type_b'.
    $this->relationships[0] = t('Any');
    foreach (\CRM_Core_PseudoConstant::relationshipType('name') as $id => $value_array) {
      if ($this->realField == 'contact_id_b') {
        $this->relationships[$id] = $value_array['name_b_a'];
      }
      else {
        $this->relationships[$id] = $value_array['name_a_b'];
      }
    }
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
    if (!empty($this->options['is_active'])) {
      $this->definition['extra'][] = array(
        'field' => 'is_active',
        'value' => TRUE,
      );
    }
    if (!empty($this->options['relationship_type']) && !array_key_exists(0, $this->options['relationship_type'])) {
      $this->definition['extra'][] = array(
        'field' => 'relationship_type_id',
        'value' => $this->options['relationship_type'],
        'numeric' => TRUE,
      );
    }
  }

  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['is_active'] = array('default' => TRUE, 'bool' => TRUE);
    $options['relationship_type'] = array('default' => 0);
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    $form['relationship_type'] = array(
      '#type' => 'select',
      '#title' => 'Relationship type',
      '#multiple' => TRUE,
      '#options' => $this->relationships,
      '#default_value' => isset($this->options['relationship_type']) ? $this->options['relationship_type'] : 0,
      '#required' => TRUE,
    );
    $form['is_active'] = array(
      '#type' => 'checkbox',
      '#title' => t('Ensure CiviCRM relationships are active?'),
      '#default_value' => isset($this->options['is_active']) ? $this->options['is_active'] : TRUE,
      '#description' => t('Uncheck this to allow listing of expired or inactive relationships in addition to current relationships.'),
    );

    parent::buildOptionsForm($form, $form_state);
  }
}