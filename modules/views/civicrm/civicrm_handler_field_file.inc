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

/**
 * Unchanged anonymous code contribution. No claim.
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
class civicrm_handler_field_file extends views_handler_field {

  public function construct() {
    parent::construct();
    civicrm_initialize();
  }

  public function render($values) {
    $value = $values->{$this->field_alias};
    $fileId = CRM_Core_DAO::singleValueQuery("SELECT file_id FROM civicrm_entity_file WHERE entity_id = %1 AND entity_table = %2", [
      1 => [$value, 'Positive'],
      2 => [$this->table_alias, 'String'],
    ]);
    if (!is_null($fileId)) {
      $path = 'civicrm/file';
      $fileType = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_File',
        $fileId,
        'mime_type'
      );

      if ($fileType && in_array($fileType, array(
        "image/jpeg", "image/pjpeg", "image/gif", "image/x-png", "image/png",
      ))) {
        $uri = CRM_Core_DAO::getFieldValue('CRM_Core_DAO_File',
          $fileId,
          'uri'
        );

        $path = sprintf('%s/imagefile', $path);
      }
      //Reset session params as we send only id and eid to retrieve the file.
      $file = new CRM_Core_Page_File();
      $file->set('filename', NULL);
      $file->set('mime-type', NULL);

      $fileHash = CRM_Core_BAO_File::generateFileHash($value, $fileId);
      return CRM_Utils_System::url($path,
        "id={$fileId}&eid={$value}&fcs={$fileHash}",
        TRUE, FALSE, FALSE, TRUE
      );
    }
    return NULL;
  }

}
