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
 * @ViewsRelationship("civicrm_uf_match")
 */
class CivicrmUFMatch extends RelationshipPluginBase {
  protected $civicrm_domains = array();
  protected $civicrm_current_domain = 1;

  public function __construct(array $configuration, $plugin_id, $plugin_definition, Civicrm $civicrm) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);

    $this->civicrm_current_domain = \CRM_Core_Config::domainID();

    $this->civicrm_domains['current'] = t('Current domain');
    $this->civicrm_domains[0] = t('All domains');
    $result = civicrm_api('domain', 'get', array('version' => 3));
    if (empty($result['is_error'])) {
      foreach ($result['values'] as $value) {
        $this->civicrm_domains[$value['id']] = $value['name'];
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

    if (!empty($this->options['civicrm_domain'])) {
      $this->definition['extra'] = array(
        array(
          'field' => 'domain_id',
          'value' => $this->options['civicrm_domain'] == 'current' ? $this->civicrm_current_domain : $this->options['civicrm_domain'],
          'numeric' => TRUE,
        ),
      );
    }
  }

  public function defineOptions() {
    $options = parent::defineOptions();
    $options['civicrm_domain'] = array('default' => 'current');
    return $options;
  }

  public function buildOptionsForm(&$form, FormStateInterface $form_state) {
    $form['civicrm_domain'] = array(
      '#type' => 'select',
      '#title' => 'Which domain of Drupal users do you want to join to?',
      '#description' => "CiviCRM can be run across multiple domains. Normally, leave this to 'current domain'.",
      '#options' => $this->civicrm_domains,
      '#default_value' => isset($this->options['civicrm_domain']) ? $this->options['civicrm_domain'] : 'current',
      '#required' => TRUE,
    );

    parent::buildOptionsForm($form, $form_state);
  }
}