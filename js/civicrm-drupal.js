(function ($) {

  // Integrate CiviCRM menu with Environment Indicator.
  Drupal.behaviors.civicrm_environment_indicator = {
    attach: function (context, settings) {
      if (typeof(CRM.config) != 'undefined' && typeof(settings.environment_indicator) != 'undefined' && typeof(settings.environment_indicator['toolbar-color']) != 'undefined') {
        $('#civicrm-menu', context).css('background-color', settings.environment_indicator['toolbar-color']);
        $('#civicrm-menu ul', context).css('background-color', settings.environment_indicator['toolbar-color']);
        $('#civicrm-menu > ul > li > a', context).css('color', settings.environment_indicator['toolbar-text-color']);
        $('#civicrm-menu .item-list', context).css('background-color', changeColor(settings.environment_indicator['toolbar-color'], 0.15, true));
        $('#civicrm-menu .item-list', context).css('background-color', changeColor(settings.environment_indicator['toolbar-color'], 0.15, true));
        $('#civicrm-menu .item-list ul li a', context).css('background-color', settings.environment_indicator['toolbar-color']).css('color', settings.environment_indicator['toolbar-text-color']);
      };
    }
  };

})(jQuery);
