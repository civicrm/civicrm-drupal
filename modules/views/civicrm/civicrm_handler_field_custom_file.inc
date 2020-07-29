<?php
/*
 +--------------------------------------------------------------------+
 | Copyright CiviCRM LLC. All rights reserved.                        |
 |                                                                    |
 | This work is published under the GNU AGPLv3 license with some      |
 | permitted exceptions and without any warranty. For full license    |
 | and copyright information, see https://civicrm.org/licensing       |
 +--------------------------------------------------------------------+
 */

/*
   / * Unchanged anonymous code contribution. No claim.
   *
   * civicrm_handler_field_custom_file.inc
   * Displays File field uri instead of id.
   *
   */

/**
 * Field handler to provide acess control for the File field
 *
 * @ingroup civicrm_field_handlers
 */
class civicrm_handler_field_custom_file extends views_handler_field {

  public function construct() {
    parent::construct();
    civicrm_initialize();
    $this->additional_fields['entity_id'] = 'entity_id';
  }

  public function render($values) {
    $value = $values->{$this->field_alias};
    if (!is_null($value)) {
      $path = 'civicrm/file';
      $fileType = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_File',
        $value,
        'mime_type'
      );

      if ($fileType && in_array($fileType, array(
        "image/jpeg", "image/pjpeg", "image/gif", "image/x-png", "image/png",
      ))) {
        $uri = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_File',
          $value,
          'uri'
        );

        $path = sprintf('%s/imagefile', $path);
      }
      $entityField = $this->aliases['entity_id'];
      $fileHash = CRM_Core_BAO_File::generateFileHash($values->$entityField, $value);
      return CRM_Utils_System::url($path,
        "id={$value}&eid={$values->$entityField}&fcs={$fileHash}",
        TRUE, NULL, FALSE, TRUE
      );
    }
    return NULL;
  }

}
