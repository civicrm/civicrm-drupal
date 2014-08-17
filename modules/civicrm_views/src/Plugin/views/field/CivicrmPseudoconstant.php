<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\civicrm\Civicrm;
use Drupal\views\ResultRow;
use Drupal\views\Plugin\views\field\FieldPluginBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Drupal\core\form\FormStateInterface;

/**
 * @Todo: offer to display raw value or human friendly value
 *
 * @ingroup views_field_handlers
 * @ViewsField("civicrm_pseudoconstant")
 */
class CivicrmPseudoconstant extends FieldPluginBase {
  protected $pseudovalues = array();

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->pseudovalues = call_user_func_array($this->definition['pseudo callback'], $this->definition['pseudo arguments']);
  }

  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('civicrm')
    );
  }

  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['pseudoconstant_format'] = array('default' => 'raw');
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    $form['pseudoconstant_format'] = array(
      '#type' => 'radios',
      '#title' => t('Display format'),
      '#description' => t("Choose how to display this field. 'Raw' will display this field as it is stored in the database, eg. as a number. 'Human friendly' will attempt to turn this raw value into something meaningful."),
      '#options' => array(
        'raw' => t('Raw value'),
        'pseudoconstant' => t('Human friendly'),
      ),
      '#default_value' => isset($this->options['pseudoconstant_format']) ? $this->options['pseudoconstant_format'] : 'raw',
    );

    parent::buildOptionsForm($form, $form_state);
  }

  public function render(ResultRow $values) {
    $value = $this->getValue($values);

    if (isset($this->options['pseudoconstant_format']) && $this->options['pseudoconstant_format'] == 'pseudoconstant') {
      if (isset($this->pseudovalues[$value])) {
        return $this->pseudovalues[$value];
      }
    }

    // Return raw value either if pseudoconstant_format is set to raw or, for some reason,
    // the raw value doesn't exist as a key in the $this->pseudovalues array.
    return $value;
  }
}