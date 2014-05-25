<?php

namespace Drupal\civicrm;

class CivicrmPageState {
  protected $title = '';
  protected $css = array();
  protected $js = array();
  protected $breadcrumbs = array();
  protected $accessDenied = FALSE;

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

  public function addBreadcrumb($name, $url) {
    $this->breadcrumbs[$name] = $url;
  }

  public function resetBreadcrumbs() {
    $this->breadcrumbs = array();
  }

  public function getBreadcrumbs() {
    return $this->breadcrumbs;
  }

  public function setAccessDenied() {
    $this->accessDenied = TRUE;
  }

  public function isAccessDenied() {
    return $this->accessDenied;
  }
}
