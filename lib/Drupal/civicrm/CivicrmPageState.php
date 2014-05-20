<?php

namespace Drupal\civicrm;

class CivicrmPageState {
  protected $title = '';
  protected $css = array();
  protected $js = array();

  public function setTitle($title) {
    $this->title = $title;
  }

  public function getTitle() {
    return $this->title;
  }

  public function addCSS($url, array $options) {
    $this->css[$url] = $options;
  }

  public function getCSS() {
    return $this->css;
  }

  public function addJS($url, array $options) {
    $this->js[$url] = $options;
  }

  public function getJS() {
    return $this->js;
  }
}
