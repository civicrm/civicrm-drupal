<?php

namespace Drupal\civicrm\Form;

use Drupal\civicrm\Civicrm;
use Drupal\Core\Access\AccessInterface;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Session\AccountInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\Routing\Exception\ResourceNotFoundException;

class UserProfile extends FormBase  {
  protected $user;
  protected $profile;
  protected $contact_id;
  protected $uf_group;

  public function __construct(Civicrm $civicrm) {
    // We don't do anything with the Civicrm service, only ensure that it
    // has been initialized.
  }

  static function create(ContainerInterface $container) {
    return new static(
      $container->get('civicrm')
    );
  }

  public function getFormId() {
    return 'civicrm_user_profile';
  }

  public function buildForm(array $form, array &$form_state, AccountInterface $user = NULL, $profile = NULL) {
    // Make the controller state available to form overrides.
    $form_state['controller'] = $this;
    $this->user = $user;
    $this->profile = $profile;

    // Search for the profile form, otherwise generate a 404.
    $uf_groups = \CRM_Core_BAO_UFGroup::getModuleUFGroup('User Account');
    if (empty($uf_groups[$profile])) {
      throw new ResourceNotFoundException();
    }
    $this->uf_group = $uf_groups[$profile];

    // Grab the form html.
    $this->contact_id = \CRM_Core_BAO_UFMatch::getContactId($user->id());
    $html = \CRM_Core_BAO_UFGroup::getEditHTML($this->contact_id, $this->uf_group['title']);

    $form['#title'] = $this->user->getUsername();
    $form['form'] = array(
      '#type' => 'fieldset',
      '#title' => $this->uf_group['title'],
      'html' => array(
        '#markup' => $html,
      ),
    );
    $form['actions'] = array(
      '#type' => 'actions',
      'submit' => array(
        '#type' => 'submit',
        '#value' => t('Save'),
        '#button_type' => 'primary',
      ),
    );

    return $form;
  }

  public function validateForm(array &$form, array &$form_state) {
    $errors = \CRM_Core_BAO_UFGroup::isValid($this->contact_id, $this->uf_group['title']);

    if (is_array($errors)) {
      foreach ($errors as $name => $error) {
        $this->setFormError($name, $form_state, $error);
      }
    }
  }

  public function submitForm(array &$form, array &$form_state) {
    // Somehow, somewhere, CiviCRM is processing our form. I have no idea how.
    drupal_set_message($this->t("Profile successfully updated."));
  }

  public function access($profile) {
    $uf_groups = \CRM_Core_BAO_UFGroup::getModuleUFGroup('User Account', 0, FALSE, \CRM_Core_Permission::EDIT);

    if (isset($uf_groups[$profile])) {
      return AccessInterface::ALLOW;
    }
    return AccessInterface::DENY;
  }
}