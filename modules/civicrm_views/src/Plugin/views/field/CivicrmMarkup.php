<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\views\Plugin\views\field\Markup;
use Drupal\core\form\FormStateInterface;
use Drupal\views\ResultRow;
use Drupal\views\Plugin\views\field\FieldPluginBase;

/**
 * @ingroup views_field_handlers
 *
 * @ViewsField("civicrm_markup")
*/
class CivicrmMarkup extends FieldPluginBase {

  protected function defineOptions() {
    $options = parent::defineOptions();
    $options['format'] = array('default' => 'plain_text');
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    $form['format'] = array(
      '#type' => 'select',
      '#title' => t('Text format'),
      '#description' => t("Select which Drupal text format to use to filter this text."),
      '#options' => array(
        'civicrm_raw' => 'Raw (bypass security filtering!)',
      ),
      '#default_value' => isset($this->options['format']) ? $this->options['format'] : 'plain_text',
    );

    $formats = filter_formats();
    foreach ($formats as $format) {
      $form['format']['#options'][$format->id()] = $format->label();
    }

    parent::buildOptionsForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function render(ResultRow $values) {
    $value = $this->getValue($values);
    if ($this->options['format'] == 'civicrm_raw') {
      return $value;
    }
    return check_markup($value, $this->options['format']);
  }
}