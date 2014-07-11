<?php

namespace Drupal\civicrm\Entity;

use Drupal\Core\Entity\ContentEntityBase;
use Drupal\Core\Entity\EntityTypeInterface;
use Drupal\Core\Field\FieldDefinition;
use Drupal\Core\Session\AccountInterface;

/**
 * Defines the civicrm_contact entity class.
 *
 * @ContentEntityType(
 *   id = "civicrm_contact",
 *   label = @Translation("CiviCRM Contact"),
 *   base_table = "civicrm_contact",
 *   fieldable = FALSE,
 *   translatable = FALSE,
 *   controllers = {
 *     "view_builder" = "Drupal\Core\Entity\EntityViewBuilder",
 *   },
 *   entity_keys = {
 *     "id" = "id",
 *     "uuid" = "id",
 *     "label" = "display_name",
 *   },
 *   links = {
 *     "canonical" = "civicrm.civicrm_contact_view",
 *   },
 * )
 */
class Contact extends ContentEntityBase {
  /**
   * {@inheritdoc}
   */
  public function access($operation = 'view', AccountInterface $account = NULL) {
    $account = $account ? $account : \Drupal::currentUser();
    return $account && $account->hasPermission('view all contacts');
  }

  /**
   * {@inheritdoc}
   */
  protected function urlRouteParameters($rel) {
    $uri_route_parameters['cid'] = $this->id();
    $uri_route_parameters['reset'] = 1;
    return $uri_route_parameters;
  }

  /**
   * {@inheritdoc}
   */
  public static function baseFieldDefinitions(EntityTypeInterface $entity_type) {
    $fields['id'] = FieldDefinition::create('integer')
      ->setLabel(t('Contact ID'))
      ->setDescription(t('The contact ID.'))
      ->setReadOnly(TRUE);

    $fields['display_name'] = FieldDefinition::create('string')
      ->setLabel(t('Display name'))
      ->setDescription(t("The contact's display name"))
      ->setReadOnly(TRUE);

    return $fields;
  }
}