<?php

namespace Drupal\civicrmtheme\Theme;

use Dompdf\Exception;
use Drupal\Core\Config\ConfigFactoryInterface;
use Drupal\Core\Routing\RouteMatchInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\Core\Theme\ThemeNegotiatorInterface;

/**
 * Theme negotiator for CiviCRM pages.
 */
class CivicrmThemeNegotiator implements ThemeNegotiatorInterface {

  /**
   * The current user.
   *
   * @var \Drupal\Core\Session\AccountInterface
   */
  protected $user;

  /**
   * The config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactoryInterface
   */
  protected $configFactory;

  /**
   * Constructs a CivicrmThemeNegotiator.
   *
   * @param \Drupal\Core\Session\AccountInterface $user
   *   The current user service.
   */
  public function __construct(AccountInterface $user, ConfigFactoryInterface $config_factory) {
    $this->user = $user;
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function applies(RouteMatchInterface $route_match) {
    $route = $route_match->getRouteObject();

    // Some pages, like 404 pages, don't have a route objet.
    if (!$route) {
      return FALSE;
    }

    $parts = explode('/', ltrim($route->getPath(), '/'));

    if ($parts[0] != 'civicrm') {
      return FALSE;
    }

    if (count($parts) > 1 && $parts[1] == 'upgrade') {
      return FALSE;
    }

    $config = $this->configFactory->get('civicrmtheme.settings');
    $admin_theme = $config->get('admin_theme');
    $public_theme = $config->get('public_theme');

    if (!$admin_theme && !$public_theme) {
      return FALSE;
    }

    // Attempt to initialize CiviCRM.
    try {
      \Drupal::service('civicrm');
    }
    catch (\Exception $e) {
      return FALSE;
    }

    return TRUE;
  }

  /**
   * {@inheritdoc}
   */
  public function determineActiveTheme(RouteMatchInterface $route_match) {
    $path = ltrim($route_match->getRouteObject()->getPath(), '/');

    // Initialize CiviCRM.
    \Drupal::service('civicrm');

    // Get the menu for above URL.
    $item = \CRM_Core_Menu::get($path);

    $config = $this->configFactory->get('civicrmtheme.settings');
    $admin_theme = $config->get('admin_theme');
    $public_theme = $config->get('public_theme');

    // Check for public pages
    // If public page and civicrm public theme is set, apply civicrm public theme
    // If user does not have access to CiviCRM use the public page for the error message
    if (!$this->user->hasPermission('access CiviCRM') || \CRM_Utils_Array::value('is_public', $item)) {
      if ($public_theme) {
        return $public_theme;
      }
    }
    elseif ($admin_theme) {
      // If admin page and civicrm admin theme is set, apply civicrm admin theme
      return $admin_theme;
    }

    return NULL;
  }

}