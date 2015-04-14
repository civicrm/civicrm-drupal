<?php

namespace Drupal\civicrm;

class CivicrmPageState {
  protected $title = '';
  protected $css = array();
  protected $js = array();
  protected $breadcrumbs = array();
  protected $accessDenied = FALSE;
  protected $html_headers = array();

  public function setTitle($title) {
    $this->title = $title;
  }

  public function getTitle() {
    return $this->title;
  }

  public function addCSS(array $css) {
    $this->css[] = $css;
  }

  public function getCSS() {
    return $this->css;
  }

  public function addJS($script) {
    $this->js[] = $script;
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

  public function addHtmlHeader($html) {
    $this->html_headers[] = $html;
  }

  public function getHtmlHeaders() {
    return implode(' ', $this->html_headers);
  }

  public function setAccessDenied() {
    $this->accessDenied = TRUE;
  }

  public function isAccessDenied() {
    return $this->accessDenied;
  }
}
