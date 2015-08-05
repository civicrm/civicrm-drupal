CiviCRM Drupal 8 Module
=======================

This is alpha version of the integration module required to allow CiviCRM and the upcoming Drupal 8 to work together. This is alpha quality: expect bugs and breakage.

It it currently verified to work against Drupal 8 beta 10 and CiviCRM 4.6.3.

Installation Steps
------------------

- Download Drupal 8 beta 10: http://ftp.drupal.org/files/projects/drupal-8.0.0-beta10.zip
- Install Drupal 8 (see https://api.drupal.org/api/drupal/core!INSTALL.txt/8 for more information).
- Create a top level `libraries` folder inside Drupal and download CiviCRM master (for Drupal 7) (from http://dist.civicrm.org/by-date/latest/master/) so that CiviCRM resides at `/libraries/civicrm`.
- Remove the Drupal 7 module folder from within CiviCRM (`libraries/civicrm/drupal`).
- Clone the Drupal 8 module into the the top level `modules` directory (this is where Drupal 8 contributed modules live now): `git clone -b 8.x-master https://github.com/civicrm/civicrm-drupal.git civicrm`
- If you want the installer to load dummy contacts and data, add the following configuration parameter to `sites/default/settings.php`: `$settings['civicrm_load_generated'] = TRUE;`
- Finally, in your browser go to `/admin/modules` and install CiviCRM Core. You should be notified of any issues that will prevent the installation from being successful, including file permissions, etc., which you will need to resolve before the installation can complete.
