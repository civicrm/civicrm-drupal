<?php

class civicrm_handler_field_contact_image extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['url_only'] = array(
      'default' => FALSE,
      'boolean' => TRUE,
    );
    $options['title'] = array(
      'default' => '',
    );
    $options['alt'] = array(
      'default' => '',
    );
    if (module_exists('image')) {
      $options['image_style'] = array(
        'default' => '',
      );
    }
    $options['width'] = array(
      'default' => '',
    );
    $options['height'] = array(
      'default' => '',
    );
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['url_only'] = array(
      '#title' => t('Output the image URL only'),
      '#description' => t('This option stops the URL being wrapped in an html <code>img</code> tag and ignores the title, alt, width and height settings. This may be useful for rewriting the results.'),
      '#type' => 'checkbox',
      '#default_value' => $this->options['url_only'],
    );

    $form['title'] = array(
      '#title' => t('Title attribute'),
      '#description' => t('The text to use as value for the <code>img</code> tag <code>title</code> attribute.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['title'],
      '#dependency' => array(
        'edit-options-url-only' => array(0),
      ),
    );

    $form['alt'] = array(
      '#title' => t('Alt attribute'),
      '#description' => t('The text to use as value for the <code>img</code> tag <code>alt</code> attribute.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['alt'],
      '#dependency' => array(
        'edit-options-url-only' => array(0),
      ),
    );

    if (module_exists('image')) {
      $styles = image_styles();
      $style_options = array('' => t('None (original image)'));
      foreach ($styles as $style) {
        $style_options[$style['name']] = $style['name'];
      }
      $form['image_style'] = array(
        '#title' => t('Image style'),
        '#description' => t('Display the contact image using an image style'),
        '#type' => 'select',
        '#options' => $style_options,
        '#default_value' => $this->options['image_style'],
        '#dependency' => array(
          'edit-options-url-only' => array(0),
        ),
      );
    }

    $form['width'] = array(
      '#title' => t('Width'),
      '#description' => t('Resize the image in the browser to this width. If left empty, the width will scale proportionally with the height.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['width'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#dependency' => array(
        'edit-options-url-only' => array(0),
      ),
    );

    $form['height'] = array(
      '#title' => t('Height'),
      '#description' => t('Resize the image in the browser to this height. If left empty, the height will scale proportionally with the width.'),
      '#type' => 'textfield',
      '#default_value' => $this->options['height'],
      '#element_validate' => array('element_validate_integer_positive'),
      '#dependency' => array(
        'edit-options-url-only' => array(0),
      ),
    );

    if (module_exists('image')) {
      $form['width']['#dependency']['edit-options-image-style'] = array('');
      $form['width']['#dependency_count'] = 2;
      $form['height']['#dependency']['edit-options-image-style'] = array('');
      $form['height']['#dependency_count'] = 2;
    }
  }

  function render($values) {
    $value = $this->get_value($values);
    if (empty($value)) {
      return;
    }
    if ($this->options['url_only']) {
      return $value;
    }
    $image = array(
      'path' => $value,
      'title' => $this->options['title'],
      'alt' => $this->options['alt'],
    );
    if ($this->options['image_style'] && module_exists('image')) {
      $image['style_name'] = empty($this->options['image_style']) ? '' : $this->options['image_style'];
      $config = CRM_Core_Config::singleton();
      $split_customFileUploadDir = explode('files', $config->customFileUploadDir);
      $image['path'] = str_replace('\\', '/', substr($split_customFileUploadDir[1], 1)) . substr(strstr($value, 'photo='), 6);
      return theme('image_style', $image);
    }
    else {
      $image['width'] = empty($this->options['width']) ? 'auto' : $this->options['width'];
      $image['height'] = empty($this->options['height']) ? 'auto' : $this->options['height'];
      return theme('image', $image);
    }
  }
}
