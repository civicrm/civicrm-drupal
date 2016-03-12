CiviCRM Drupal 8 Module
=======================

This is alpha version of the integration module required to allow CiviCRM and Drupal 8 to work together. This is not the stable version yet.

It it currently verified to work against Drupal 8.0.5 and CiviCRM 4.7.3.

Installation Steps
------------------

- Download the latest Drupal 8 release: (from https://www.drupal.org/project/drupal).
- Install Drupal 8 (see https://api.drupal.org/api/drupal/core!INSTALL.txt/8 for more information).
- Create a top level `libraries` folder inside Drupal and download CiviCRM master (for Drupal 7) (from http://dist.civicrm.org/by-date/latest/master/) so that CiviCRM resides at `/libraries/civicrm`. CiviCRM can also be installed in /modules/civicrm - see CRM-18222.
- If it exists, remove the Drupal 7 module folder from within CiviCRM (`libraries/civicrm/drupal`).
- Clone the Drupal 8 module into the the top level `modules` directory (this is where Drupal 8 contributed modules live now): `git clone -b 8.x-master https://github.com/civicrm/civicrm-drupal.git civicrm`
- Edit civicrm-version.php and change the 'cms' value from 'Drupal' to 'Drupal8'. If you are reading this after having already installed CiviCRM and running into an undefined `arg(0)` error, change the CIVICRM_UF value in your civicrm.settings.php file.
- If you want the installer to load dummy contacts and data, add the following configuration parameter to `sites/default/settings.php`: `$settings['civicrm_load_generated'] = TRUE;`
- Finally, in your browser go to `/admin/modules` and install CiviCRM Core. You should be notified of any issues that will prevent the installation from being successful, including file permissions, etc., which you will need to resolve before the installation can complete.
