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

/**
 * @file
 * The subclass simply adds properties,
 * for field-specific subclasses to use if they need to.
 */
class civicrm_cck_handler_filter extends views_handler_filter_in_operator {
  var $no_single = TRUE;
  function value_form(&$form, &$form_state) {
    $values  = array();
    $field   = $this->definition['content_field_name'];
    $options = civicrm_cck_allowed_values($field);

    sort($values);
    $default_value = implode(', ', $values);
    $form['value'] = array(
      '#type' => 'textfield',
      '#title' => t('Contact Name'),
      '#description' => t('Enter a contact name.'),
      '#default_value' => $default_value,
      '#autocomplete_path' => 'civicrm_cck/autocomplete/' . $field,
    );

    if (!empty($form_state['exposed']) && !isset($form_state['input'][$this->options['expose']['identifier']])) {
      $form_state['input'][$this->options['expose']['identifier']] = $default_value;
    }
  }

  function value_validate(&$form, &$form_state) {
    //$values = drupal_explode_tags($form_state['values']['options']['value']);
    $values = $form_state['values']['options']['value'];
    $cids = $this->validate_contact_strings($form['value'], $values);

    if ($cids) {
      $form_state['values']['options']['value'] = $cids;
    }
  }

  function accept_exposed_input($input) {
    $rc = parent::accept_exposed_input($input);

    if ($rc) {
      // If we have previously validated input, override.
      if (isset($this->validated_exposed_input)) {
        $this->value = $this->validated_exposed_input;
      }
    }

    return $rc;
  }

  function exposed_validate(&$form, &$form_state) {
    if (empty($this->options['exposed'])) {
      return;
    }

    if (empty($this->options['expose']['identifier'])) {
      return;
    }

    $identifier = $this->options['expose']['identifier'];
    $values = $form_state['values'][$identifier];

    $cids = $this->validate_contact_strings($form[$identifier], $values);

    if ($cids) {
      $this->validated_exposed_input = $cids;
    }
  }

  /**
   * Validate the user string. Since this can come from either the form
   * or the exposed filter, this is abstracted out a bit so it can
   * handle the multiple input sources.
   */
  function validate_contact_strings(&$form, $value) {

    $cids = array();

    preg_match('/^(?:\s*|(.*) )?\[\s*cid\s*:\s*(\d+)\s*\]$/', $value, $matches);

    if (!empty($matches)) {
      // Explicit [cid:n].
      list(, $title, $cid) = $matches;
      $cids[] = $cid;
    }
    else {
      // No explicit nid.
      $references = _civicrm_cck_potential_references($field, $value, TRUE);
      if (empty($references)) {
        //    form_error($element[$field_key], t('%name: found no valid post with that title.', array('%name' => $field['widget']['label'])));
      }
      else {
        // TODO:
        // the best thing would be to present the user with an additional form,
        // allowing the user to choose between valid candidates with the same title
        // ATM, we pick the first matching candidate...
        $cids[] = array_shift(array_keys($references));
      }
    }

    return $cids;
  }

  // Override to do nothing.
  function value_submit() {}

  // Override to do nothing.
  function get_value_options() {}
}

