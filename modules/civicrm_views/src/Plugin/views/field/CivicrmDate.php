<?php

namespace Drupal\civicrm_views\Plugin\views\field;

use Drupal\views\Plugin\views\field\Date;
use Drupal\views\ResultRow;

/**
 * @ingroup views_field_handlers
 *
 * @ViewsField("civicrm_date")
 */
class CivicrmDate extends Date {
  /**
   * {@inheritdoc}
   */
  public function getValue(ResultRow $values, $field = NULL) {
    $value = parent::getValue($values, $field);
    return strtotime($value);
  }
}