<?php
// $Id$

/*
 +--------------------------------------------------------------------+
 | CiviCRM version 3.4                                                |
 +--------------------------------------------------------------------+
 | This file is a part of CiviCRM.                                    |
 |                                                                    |
 | CiviCRM is free software; you can copy, modify, and distribute it  |
 | under the terms of the GNU Affero General Public License           |
 | Version 3, 19 November 2007 and the CiviCRM Licensing Exception.   |
 |                                                                    |
 | CiviCRM is distributed in the hope that it will be useful, but     |
 | WITHOUT ANY WARRANTY; without even the implied warranty of         |
 | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.               |
 | See the GNU Affero General Public License for more details.        |
 |                                                                    |
 | You should have received a copy of the GNU Affero General Public   |
 | License and the CiviCRM Licensing Exception along                  |
 | with this program; if not, contact CiviCRM LLC                     |
 | at info[AT]civicrm[DOT]org. If you have questions about the        |
 | GNU Affero General Public License or the licensing of CiviCRM,     |
 | see the CiviCRM license FAQ at http://civicrm.org/licensing        |
 +--------------------------------------------------------------------+
*/
function civicrm_views_default_views() {
  $views = NULL;
  if (module_exists('date_api') && module_exists('calendar')) {
    $view               = new view;
    $view->name         = 'civicrm_events';
    $view->description  = 'CiviCRM Event Calendar';
    $view->tag          = 'CiviEvent';
    $view->view_php     = '';
    $view->base_table   = 'civicrm_event';
    $view->is_cacheable = FALSE;
    $view->api_version  = 2;
    /* Edit this to true to make a default view disabled initially */
    $view->disabled = TRUE;
    $handler = $view->new_display('default', 'Defaults', 'default');
    $handler->override_option('fields', array(
        'start_date' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'date_format' => 'medium',
          'custom_date_format' => '',
          'exclude' => 0,
          'id' => 'start_date',
          'table' => 'civicrm_event',
          'field' => 'start_date',
          'relationship' => 'none',
        ),
        'title' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'link_to_civicrm_event' => 'page',
          'custom_link' => '',
          'exclude' => 0,
          'id' => 'title',
          'table' => 'civicrm_event',
          'field' => 'title',
          'relationship' => 'none',
        ),
      ));
    $handler->override_option('sorts', array(
        'start_date' => array(
          'order' => 'ASC',
          'granularity' => 'second',
          'id' => 'start_date',
          'table' => 'civicrm_event',
          'field' => 'start_date',
          'relationship' => 'none',
        ),
      ));
    $handler->override_option('arguments', array(
        'date_argument' => array(
          'default_action' => 'default',
          'style_plugin' => 'default_summary',
          'style_options' => array(),
          'wildcard' => 'all',
          'wildcard_substitution' => 'All',
          'title' => '',
          'default_argument_type' => 'date',
          'default_argument' => '',
          'validate_type' => 'none',
          'validate_fail' => 'not found',
          'date_fields' => array(
            'civicrm_event.start_date' => 'civicrm_event.start_date',
          ),
          'year_range' => '-3:+3',
          'date_method' => 'OR',
          'granularity' => 'month',
          'id' => 'date_argument',
          'table' => 'civicrm_event',
          'field' => 'date_argument',
          'validate_user_argument_type' => 'uid',
          'validate_user_roles' => array(
            '2' => 0,
            '8' => 0,
            '12' => 0,
            '14' => 0,
            '9' => 0,
            '15' => 0,
            '10' => 0,
            '5' => 0,
            '4' => 0,
            '7' => 0,
            '11' => 0,
            '6' => 0,
            '13' => 0,
          ),
          'relationship' => 'none',
          'default_options_div_prefix' => '',
          'default_argument_user' => 0,
          'default_argument_fixed' => '',
          'default_argument_php' => '',
          'validate_argument_node_type' => array(
            'webform' => 0,
            'blog' => 0,
            'broadcast' => 0,
            'class' => 0,
            'feed' => 0,
            'icalfeed' => 0,
            'page' => 0,
            'podcast' => 0,
            'story' => 0,
            'video_upload' => 0,
            'wiki' => 0,
          ),
          'validate_argument_node_access' => 0,
          'validate_argument_nid_type' => 'nid',
          'validate_argument_vocabulary' => array(
            '2' => 0,
            '3' => 0,
            '6' => 0,
            '4' => 0,
            '5' => 0,
          ),
          'validate_argument_type' => 'tid',
          'validate_argument_transform' => 0,
          'validate_user_restrict_roles' => 0,
          'validate_argument_php' => '',
        ),
      ));
    $handler->override_option('filters', array(
        'is_active' => array(
          'operator' => '=',
          'value' => '1',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'is_active',
          'table' => 'civicrm_event',
          'field' => 'is_active',
          'relationship' => 'none',
        ),
        'is_public' => array(
          'operator' => '=',
          'value' => '1',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'is_public',
          'table' => 'civicrm_event',
          'field' => 'is_public',
          'relationship' => 'none',
        ),
        'is_template' => array(
          'operator' => '=',
          'value' => '0',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'is_template',
          'table' => 'civicrm_event',
          'field' => 'is_template',
          'relationship' => 'none',
        ),
      ));
    $handler->override_option('access', array(
        'type' => 'none',
      ));
    $handler->override_option('title', 'Events');
    $handler->override_option('header_empty', 1);
    $handler->override_option('items_per_page', 0);
    $handler->override_option('use_more', 0);
    $handler->override_option('style_plugin', 'calendar_nav');
    $handler = $view->new_display('calendar', 'Events page', 'calendar_1');
    $handler->override_option('path', 'events');
    $handler->override_option('menu', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
        'name' => 'navigation',
      ));
    $handler->override_option('tab_options', array(
        'type' => 'none',
        'title' => '',
        'description' => '',
        'weight' => 0,
      ));
    $handler->override_option('calendar_colors', array(
        '0' => array(),
      ));
    $handler->override_option('calendar_colors_vocabulary', array());
    $handler->override_option('calendar_colors_taxonomy', array());
    $handler->override_option('calendar_popup', 0);
    $handler->override_option('calendar_date_link', '');
    $handler = $view->new_display('calendar_block', 'Events block', 'calendar_block_1');
    $handler->override_option('block_description', 'Events Calendar');
    $handler->override_option('block_caching', -1);
    $handler = $view->new_display('calendar_period', 'Year view', 'calendar_period_1');
    $handler->override_option('style_plugin', 'calendar_style');
    $handler->override_option('style_options', array(
        'display_type' => 'year',
        'name_size' => 1,
        'max_items' => 0,
      ));
    $handler->override_option('attachment_position', 'after');
    $handler->override_option('inherit_arguments', TRUE);
    $handler->override_option('inherit_exposed_filters', TRUE);
    $handler->override_option('displays', array(
        'calendar_1' => 'calendar_1',
        'default' => 0,
        'calendar_block_1' => 0,
      ));
    $handler->override_option('calendar_type', 'year');
    $handler = $view->new_display('calendar_period', 'Month view', 'calendar_period_2');
    $handler->override_option('style_plugin', 'calendar_style');
    $handler->override_option('style_options', array(
        'display_type' => 'month',
        'name_size' => '99',
        'with_weekno' => '1',
        'date_fields' => NULL,
        'max_items' => 0,
      ));
    $handler->override_option('attachment_position', 'after');
    $handler->override_option('inherit_arguments', TRUE);
    $handler->override_option('inherit_exposed_filters', TRUE);
    $handler->override_option('displays', array(
        'calendar_1' => 'calendar_1',
        'default' => 0,
        'calendar_block_1' => 0,
      ));
    $handler->override_option('calendar_type', 'month');
    $handler = $view->new_display('calendar_period', 'Day view', 'calendar_period_3');
    $handler->override_option('style_plugin', 'calendar_style');
    $handler->override_option('style_options', array(
        'name_size' => '99',
        'with_weekno' => 0,
        'max_items' => 0,
        'max_items_behavior' => 'more',
        'groupby_times' => 'hour',
        'groupby_times_custom' => '',
        'groupby_field' => '',
      ));
    $handler->override_option('attachment_position', 'after');
    $handler->override_option('inherit_arguments', TRUE);
    $handler->override_option('inherit_exposed_filters', TRUE);
    $handler->override_option('displays', array(
        'calendar_1' => 'calendar_1',
        'default' => 0,
        'calendar_block_1' => 0,
      ));
    $handler->override_option('calendar_type', 'day');
    $handler = $view->new_display('calendar_period', 'Week view', 'calendar_period_4');
    $handler->override_option('style_plugin', 'calendar_style');
    $handler->override_option('style_options', array(
        'name_size' => '99',
        'with_weekno' => 0,
        'max_items' => 0,
        'max_items_behavior' => 'more',
        'groupby_times' => 'hour',
        'groupby_times_custom' => '',
        'groupby_field' => '',
      ));
    $handler->override_option('attachment_position', 'after');
    $handler->override_option('inherit_arguments', TRUE);
    $handler->override_option('inherit_exposed_filters', TRUE);
    $handler->override_option('displays', array(
        'calendar_1' => 'calendar_1',
        'default' => 0,
        'calendar_block_1' => 0,
      ));
    $handler->override_option('calendar_type', 'week');
    $handler = $view->new_display('calendar_period', 'Block view', 'calendar_period_5');
    $handler->override_option('style_plugin', 'calendar_style');
    $handler->override_option('style_options', array(
        'display_type' => 'month',
        'name_size' => '1',
      ));
    $handler->override_option('attachment_position', 'after');
    $handler->override_option('inherit_arguments', TRUE);
    $handler->override_option('inherit_exposed_filters', TRUE);
    $handler->override_option('displays', array(
        'calendar_1' => 0,
        'default' => 0,
        'calendar_block_1' => 'calendar_block_1',
      ));
    $handler->override_option('calendar_type', 'month');
    $handler = $view->new_display('block', 'Upcoming Events', 'block_1');
    $handler->override_option('fields', array(
        'start_date' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'date_format' => 'medium',
          'custom_date_format' => '',
          'exclude' => 0,
          'id' => 'start_date',
          'table' => 'civicrm_event',
          'field' => 'start_date',
          'relationship' => 'none',
        ),
        'title' => array(
          'label' => '',
          'alter' => array(
            'alter_text' => 0,
            'text' => '',
            'make_link' => 0,
            'path' => '',
            'alt' => '',
            'prefix' => '',
            'suffix' => '',
            'help' => '',
            'trim' => 0,
            'max_length' => '',
            'word_boundary' => 1,
            'ellipsis' => 1,
            'strip_tags' => 0,
            'html' => 0,
          ),
          'link_to_civicrm_event' => 'page',
          'custom_link' => '',
          'exclude' => 0,
          'id' => 'title',
          'table' => 'civicrm_event',
          'field' => 'title',
          'relationship' => 'none',
        ),
      ));
    $handler->override_option('arguments', array());
    $handler->override_option('filters', array(
        'is_active' => array(
          'operator' => '=',
          'value' => '1',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'is_active',
          'table' => 'civicrm_event',
          'field' => 'is_active',
          'relationship' => 'none',
        ),
        'is_public' => array(
          'operator' => '=',
          'value' => '1',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'is_public',
          'table' => 'civicrm_event',
          'field' => 'is_public',
          'relationship' => 'none',
        ),
        'is_template' => array(
          'operator' => '=',
          'value' => '0',
          'group' => '0',
          'exposed' => FALSE,
          'expose' => array(
            'operator' => FALSE,
            'label' => '',
          ),
          'id' => 'is_template',
          'table' => 'civicrm_event',
          'field' => 'is_template',
          'relationship' => 'none',
        ),
      ));
    $handler->override_option('title', 'Upcoming Events');
    $handler->override_option('items_per_page', 5);
    $handler->override_option('use_more', 1);
    $handler->override_option('style_plugin', 'list');
    $handler->override_option('style_options', array(
        'grouping' => '',
        'type' => 'ul',
      ));
    $handler->override_option('block_description', 'Upcoming Events');
    $handler->override_option('block_caching', -1);

    $views[$view->name] = $view;
  }

  return $views;
}

