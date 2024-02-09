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
 *
 * @package CRM
 * @copyright CiviCRM LLC https://civicrm.org/licensing
 */

/**
 * @file
 *   Example drush command.
 *
 *   Shows how to make your own drush command.
 *
 *   You can copy this file to any of the following
 *     1. A .drush folder in your HOME folder.
 *     2. Anywhere in a folder tree below an active module on your site.
 *     3. In an arbitrary folder specified with the --include option.
 */

/**
 * Implements hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and
 * description.
 *
 * Notice how this structure closely resembles how
 * you define menu hooks.
 *
 * @See drush_parse_command() for a list of recognized keys.
 *
 */
function civicrm_drush_command() {
  $items = [];

  // the key in the $items array is the name of the command.
  $items['civicrm-api'] = [
    'description' => 'CLI access to CiviCRM APIs. It can return pretty-printor json formatted data.',
    'examples' => [
      'drush civicrm-api contact.create first_name=John last_name=Doe contact_type=Individual' => 'Create a new contact named John Doe',
      'drush civicrm-api contact.create id=1 --out=json' => 'Find/display a contact in JSON format',
    ],
    'options' => [
      'in' => 'Input type: "args" (command-line), "json" (STDIN)',
      'out' => 'Output type: "pretty" (STDOUT), "json" (STDOUT)',
    ],
    'aliases' => ['cvapi'],
  ];
  $items['civicrm-install'] = [
    'description' => 'Install a new instance of CiviCRM.',
    'options' => [
      'dbuser' => 'MySQL username for your Drupal/CiviCRM database.',
      'dbpass' => 'MySQL password for your Drupal/CiviCRM database.',
      'dbhost' => 'MySQL host for your Drupal/CiviCRM database. Defaults to localhost.',
      'dbname' => 'MySQL database name of your Drupal/CiviCRM database.',
      'tarfile' => 'Path to your CiviCRM tar.gz file.',
      'destination' => 'Destination modules path to extract CiviCRM (eg : sites/all/modules ).',
      'lang' => 'Default language to use for installation.',
      'langtarfile' => 'Path to your l10n tar.gz file.',
      'site_url' => 'Base Url for your drupal/CiviCRM website without http (e.g. mysite.com)',
      'ssl' => 'Using ssl for your drupal/CiviCRM website if set to on (e.g. --ssl=on)',
    ],
    'aliases' => ['cvi'],
  ];
  $items['civicrm-ext-list'] = [
    'description' => "List of CiviCRM extensions enabled.",
    'options' => [
      'status' => 'Filter by extension status (installed, uninstalled, disabled).',
      'out' => 'Output type: "pretty" (STDOUT) by default, "json" (STDOUT)',
    ],
    'examples' => [
      'Standard example' => 'drush civicrm-ext-list',
      'Display installed extensions' => 'drush civicrm-ext-list --status=installed',
      'Display disabled extensions as json' => 'drush civicrm-ext-list --status=disabled --out=json',
    ],
    'aliases' => ['cel'],
  ];
  $items['civicrm-ext-install'] = [
    'description' => 'Install a CiviCRM extension.',
    'arguments' => [
      'ename' => 'Extension name.',
    ],
    'required-arguments' => TRUE,
    'examples' => [
      'Standard example' => 'drush civicrm-ext-install civimobile',
    ],
    'aliases' => ['cei'],
  ];
  $items['civicrm-ext-disable'] = [
    'description' => 'Disable a CiviCRM extension.',
    'arguments' => [
      'ename' => 'Extension name.',
    ],
    'required-arguments' => TRUE,
    'examples' => [
      'Standard example' => 'drush civicrm-ext-disable civimobile',
    ],
    'aliases' => ['ced'],
  ];
  $items['civicrm-ext-uninstall'] = [
    'description' => 'Uninstall a CiviCRM extension.',
    'arguments' => [
      'ename' => 'Extension name.',
    ],
    'required-arguments' => TRUE,
    'examples' => [
      'Standard example' => 'drush civicrm-ext-uninstall civimobile',
    ],
    'aliases' => ['ceui'],
  ];

  $items['civicrm-upgrade-db'] = [
    'description' => 'Execute the civicrm/upgrade?reset=1 process from the command line.',
    'aliases' => ['cvupdb'],
  ];
  $items['civicrm-update-cfg'] = [
    'description' => 'Update config_backend to correct config settings, especially when the CiviCRM site has been cloned / migrated.',
    'examples' => [
      'drush -l http://example.com/civicrm civicrm-update-cfg' => 'Update config_backend to correct config settings for civicrm installation on example.com site.',
    ],
    'aliases' => ['cvupcfg'],
  ];
  $items['civicrm-enable-debug'] = [
    'description' => "Enable CiviCRM Debugging.",
  ];
  $items['civicrm-disable-debug'] = [
    'description' => "Disable CiviCRM Debugging.",
  ];
  $items['civicrm-pipe'] = [
    'description' => 'Start a Civi::pipe session (JSON-RPC 2.0)',
    'examples' => [
      'drush civicrm-pipe' => 'Begin a session with default flags',
      'drush civicrm-pipe vt' => 'Begin a session with connection flags (ex: version, trusted)',
      'drush civicrm-pipe vu' => 'Begin a session with connection flags (ex: version, untrusted)',
    ],
    'arguments' => [
      'connection-flags' => 'List of connection flags (https://docs.civicrm.org/dev/en/latest/framework/pipe#flags)',
    ],
    'aliases' => ['cvpipe'],
  ];
  $items['civicrm-upgrade'] = [
    'description' => 'Replace CiviCRM codebase with new specified tarfile and upgrade database by executing the CiviCRM upgrade process - civicrm/upgrade?reset=1.',
    'examples' => [
      'drush civicrm-upgrade --tarfile=~/tarballs/civicrm-4.1.2-drupal.tar.gz' => 'Replace old CiviCRM codebase with new v4.1.2 and run upgrade process.',
    ],
    'options' => [
      'tarfile' => 'Path of new CiviCRM tarfile, with which current CiviCRM codebase is to be replaced.',
      'backup-dir' => 'Specify a directory to backup current CiviCRM codebase and database into, defaults to a backup directory above your Drupal root.',
    ],
    'aliases' => ['cvup'],
  ];
  $items['civicrm-restore'] = [
    'description' => 'Restore CiviCRM codebase and database back from the specified backup directory.',
    'examples' => [
      'drush civicrm-restore --restore-dir=../backup/modules/20100309200249' => 'Replace current civicrm codebase with the $restore-dir/civicrm codebase, and reload the database with $restore-dir/civicrm.sql file',
    ],
    'options' => [
      'restore-dir' => 'Path of directory having backed up CiviCRM codebase and database.',
      'backup-dir' => 'Specify a directory to backup current CiviCRM codebase and database into, defaults to a backup directory above your Drupal root.',
    ],
  ];
  $items['civicrm-rest'] = [
    'description' => 'Rest interface for accessing CiviCRM APIs. It can return xml or json formatted data.',
    'examples' => [
      "drush civicrm-rest --query='civicrm/contact/search&json=1&key=7decb879f28ac4a0c6a92f0f7889a0c9&api_key=7decb879f28ac4a0c6a92f0f7889a0c9'" => 'Use contact search api to return data in json format.',
    ],
    // TODO: This really makes more sense as an argument.
    'options' => ['query' => 'Query part of url. Refer CiviCRM wiki doc for more details.'],
    'aliases' => ['cvr'],
  ];
  $items['civicrm-sql-conf'] = [
    // explicit callback declaration and non-standard name to avoid collision with "sql-conf"
    'callback' => 'drush_civicrm_sqlconf',
    'description' => 'Print CiviCRM database connection details.',
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
  ];
  $items['civicrm-sql-connect'] = [
    // explicit callback declaration and non-standard name to avoid collision with "sql-connect"
    'callback' => 'drush_civicrm_sqlconnect',
    'description' => 'A string for connecting to the CiviCRM DB.',
  ];
  $items['civicrm-sql-dump'] = [
    // explicit callback declaration and non-standard name to avoid collision with "sql-dump"
    'callback' => 'drush_civicrm_sqldump',
    'description' => 'Exports the CiviCRM DB as SQL using mysqldump.',
    'examples' => [
      'drush civicrm-sql-dump --result-file=../CiviCRM.sql' => 'Save SQL dump to the directory above Drupal root.',
      'drush civicrm-sql-dump --extra-options=--quick' => 'Pass the --quick option to mysqldump to help with large tables.',
    ],
    'options' => [
      'data-only' => 'Dump data without statements to create any of the schema.',
      'gzip' => 'Compress the dump using the gzip program which must be in your $PATH.',
      'result-file' => 'Save to a file.',
      'tables-list' => 'comma-separated list of tables to transfer.',
      'extra-options' => 'Add custom options to the dump command.',
    ],
  ];
  $items['civicrm-sql-query'] = [
    // explicit callback declaration and non-standard name to avoid collision with "sql-query"
    'callback' => 'drush_civicrm_sqlquery',
    'description' => 'Execute a query against the CiviCRM database.',
    'examples' => [
      'drush civicrm-sql-query "SELECT * FROM civicrm_contact WHERE id=1"' => 'Browse user record',
      'drush civicrm-sql-query --file=example.sql' => 'Alternate way to import sql statements from a file.',
    ],
    'arguments' => [
      'query' => 'A SQL query. Ignored if \'file\' is provided.',
    ],
    'options' => [
      'file' => 'Path to a file containing the SQL to be run. ',
    ],
  ];
  $items['civicrm-sql-cli'] = [
    // explicit callback declaration and non-standard name to avoid collision with "sql-cli"
    'callback' => 'drush_civicrm_sqlcli',
    'description' => "Open a SQL command-line interface using CiviCRM's credentials.",
    'aliases' => ['cvsqlc'],
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_CONFIGURATION,
  ];
  $items['civicrm-sql-rebuild-triggers'] = [
    'description' => 'Rebuild SQL triggers',
    'aliases' => ['cvsqlrt'],
    'options' => [
      'table' => 'Specific table to target. If omitted, triggers for all tables are rebuilt.',
    ],
  ];
  $items['civicrm-process-mail-queue'] = [
    'description' => "Process pending CiviMail mailing jobs.",
    'examples' => [
      'drush civicrm-process-mail-queue -u admin' => 'Process CiviMail queue with admin credentials.',
    ],
  ];
  $items['civicrm-member-records'] = [
    'description' => "Run the CiviMember UpdateMembershipRecord cron (civicrm-member-records).",
  ];
  $items['civicrm-sync-users-contacts'] = [
    'description' => "Synchronize Users to Contacts: CiviCRM will check each user record for a contact record. A new contact record will be created for each user where one does not already exist.",
  ];

  return $items;
}

/**
 * Implementation of database specification for the active DB connection
 */
function drush_civicrm_get_db_spec() {
  if (version_compare(DRUSH_VERSION, 7, '>=')) {
    $sql = drush_sql_get_class();
    $db_spec = $sql->db_spec();
  }
  else {
    $db_spec = _drush_sql_get_db_spec();
  }
  return $db_spec;
}

/**
 * Implements hook_COMMAND_validate() for civicrm-install().
 */
function drush_civicrm_install_validate() {
  // TODO: Replace these with required options (Drush 5).
  // Get the drupal credentials in case civi specific db info is not passed.
  if (drush_get_option('db-url', FALSE)) {
    $db_spec['db-url'] = $GLOBALS['db_url'];
  }
  elseif (drush_get_option('all', FALSE)) {
    $db_spec = _drush_sql_get_all_db_specs();
  }
  if (!isset($db_spec)) {
    $db_spec = drush_civicrm_get_db_spec();
  }

  if (!drush_get_option('dbuser', FALSE)) {
    drush_set_option('dbuser', $db_spec['username']);
  }
  if (!drush_get_option('dbpass', FALSE)) {
    drush_set_option('dbpass', $db_spec['password']);
  }
  if (!drush_get_option('dbhost', FALSE)) {
    $suffix = empty($db_spec['port']) ? '' : (':' . $db_spec['port']);
    drush_set_option('dbhost', $db_spec['host'] . $suffix);
  }
  if (!drush_get_option('dbname', FALSE)) {
    drush_set_option('dbname', $db_spec['database']);
  }

  $crmpath = _civicrm_get_crmpath();
  $drupalRoot = drush_get_context('DRUSH_DRUPAL_ROOT');
  $modPath = "$drupalRoot/$crmpath";

  if (!is_dir("$modPath/civicrm") && !drush_get_option('tarfile', FALSE)) {
    return drush_set_error('CIVICRM_INSTALL_TARFILE_NOT_SPECIFIED', dt('CiviCRM tarfile not specified.'));
  }
  if (drush_get_option('lang', FALSE) && !drush_get_option('langtarfile', FALSE)) {
    return drush_set_error('CIVICRM_INSTALL_LANGTARFILE_NOT_SPECIFIED', dt('CiviCRM language tarfile not specified.'));
  }

  return TRUE;
}

/**
 * Implementation of command 'civicrm-install'
 */
function drush_civicrm_install() {
  $dbuser = drush_get_option('dbuser', FALSE);
  $dbpass = drush_get_option('dbpass', FALSE);
  $dbhost = drush_get_option('dbhost', FALSE);
  $dbname = drush_get_option('dbname', FALSE);

  $crmpath = _civicrm_get_crmpath();
  $drupalRoot = drush_get_context('DRUSH_DRUPAL_ROOT');
  $modPath = "$drupalRoot/$crmpath";
  $lang = drush_get_option('lang', '');

  if (!is_dir("$modPath/civicrm")) {
    // extract tarfile at right place
    _civicrm_extract_tarfile($modPath);
    drush_log(dt('Tarfile unpacked.'), 'ok');
  }

  if ($lang != '') {
    _civicrm_extract_tarfile($modPath, "langtarfile");
  }

  // Invokes \Civi\Setup
  // Function name kept somewhat for backwards compatibility (ex: Aegir)
  _civicrm_install_db($dbuser, $dbpass, $dbhost, $dbname, $modPath, $lang);

  drush_log(dt("CiviCRM installed."), 'ok');
}

function _civicrm_extract_tarfile($destinationPath, $option = 'tarfile') {
  $tarpath = drush_get_option($option, FALSE);
  if (drush_shell_exec("gzip -d " . $tarpath)) {
    $tarpath = preg_replace('/(tar\.gz|tgz)$/', 'tar', $tarpath);
  }
  drush_shell_exec("tar -xf $tarpath -C \"$destinationPath\"");
}

/**
 * Wrapper around \Civi\Setup, kept for backwards compatibility
 *
 * @var string|null $dbuser
 * @var string|null $dbpass
 * @var string|null $dbhost
 * @var string|null $dbname
 * @var string|null $modPath
 *       This variable is not used. Kept for backwards-compatibility only.
 * @var string|null $lang
 *       CiviCRM default language for the interface and defaults configurations.
 *       The installer defaults to en_US.
 */
function _civicrm_install_db($dbuser = NULL, $dbpass = NULL, $dbhost = NULL, $dbname = NULL, $modPath = NULL, $lang = NULL) {
  $coreUrl = dirname(file_create_url(drupal_get_path('module', 'civicrm')));
  $corePath = dirname(dirname(__DIR__));
  $classLoader = implode(DIRECTORY_SEPARATOR, [$corePath, 'CRM', 'Core', 'ClassLoader.php']);

  if (file_exists($classLoader)) {
    require_once $classLoader;
    CRM_Core_ClassLoader::singleton()->register();
    $log = new CiviCRMDrushLog();
    \Civi\Setup::assertProtocolCompatibility(1.0);
    \Civi\Setup::init([
      // This is just enough information to get going. Drupal.civi-setup.php does more scanning.
      'cms' => 'Drupal',
      'srcPath' => $corePath,
    ], NULL, $log);

    // init() made the initial guess. Now we can overwrite with user-supplied data.
    $setup = \Civi\Setup::instance();
    $model = $setup->getModel();
    $model->db = [
      'server' => $dbhost,
      'username' => $dbuser,
      'password' => $dbpass,
      'database' => $dbname,
    ];
    if ($lang) {
      $model->lang = $lang;
    }

    // Validate system requirements
    $reqs = $setup->checkRequirements();
    foreach ($reqs->getWarnings() as $msg) {
      drush_log(dt("WARNING: (@section) @name: @message", ['@section' => $msg['section'], '@name' => $msg['name'], '@message' => $msg['message']]), 'warning');
    }
    $errors = $reqs->getErrors();
    if ($errors) {
      foreach ($errors as $msg) {
        drush_log(dt("ERROR: (@section) @name: @message", ['@section' => $msg['section'], '@name' => $msg['name'], '@message' => $msg['message']]), 'error');
      }
      drush_set_error('CIVICRM_INSTALL_REQUIREMENTS_FAILED', dt('Requirements check failed.'));
      return;
    }

    $installed = $setup->checkInstalled();
    if (!$installed->isSettingInstalled()) {
      drush_log(dt("Creating file @file", ['@file' => $setup->getModel()->settingsPath]), 'info');
      $setup->installFiles();
    }
    else {
      drush_log(dt("Found existing @file in @dir", ['@file' => basename($setup->getModel()->settingsPath), '@dir' => dirname($setup->getModel()->settingsPath)]), 'error');
      drush_set_error('CIVICRM_INSTALL_SETTINGS_EXIST', dt('Found existing civicrm.settings.php file.'));
      return;
    }

    if (!$installed->isDatabaseInstalled()) {
      drush_log(dt("Creating <comment>civicrm_*</comment> database tables in @db", ['@db' => $setup->getModel()->db['database']]), 'info');
      $setup->installDatabase();
    }
  }
  else {
    return drush_set_error('CIVICRM_NOT_PRESENT', dt('Cannot perform setup for CiviCRM. The file "@file" is missing..', ['@file' => $classLoader]));
  }

  drush_log(dt('CiviCRM database loaded successfully.'), 'ok');
}

/**
 * Kept only for backwards compatiblity
 * File creation is now part of \Civi\Setup
 */
function _civicrm_create_files_dirs($civicrmInstallerHelper, $modPath) {
  drush_log(dt("CiviCRM: function _civicrm_create_files_dirs is deprecated. Use \\Civi\\Setup installFiles"), 'warning');
  return TRUE;
}

/**
 * Generates civicrm.settings.php file
 */
function _civicrm_generate_settings_file($dbuser, $dbpass, $dbhost, $dbname, $modPath) {
  drush_log(dt("CiviCRM: function _civicrm_generate_settings_file is deprecated. Use \\Civi\\Setup installFiles"), 'warning');
  return TRUE;
}

/**
 * Implements hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 */
function civicrm_drush_help($section) {
  switch ($section) {
    case 'drush:civicrm-upgrade-db':
      return dt('Run civicrm/upgrade?reset=1 just as a web browser would.');

    case 'drush:civicrm-update-cfg':
      return dt("Update config_backend to correct config settings, especially when the CiviCRM site has been cloned / migrated.");

    case 'drush:civicrm-upgrade':
      return dt('Take backups, replace CiviCRM codebase with new specified tarfile and upgrade database by executing the CiviCRM upgrade process - civicrm/upgrade?reset=1. Use civicrm-restore to revert to previous state in case anything goes wrong.');

    case 'drush:civicrm-restore':
      return dt("Restore CiviCRM codebase and database back from the specified backup directory.");

    case 'drush:civicrm-rest':
      return dt("Rest interface for accessing CiviCRM APIs. It can return xml or json formatted data.");

    case 'drush:civicrm-sql-conf':
      return dt('Show civicrm database connection details.');

    case 'drush:civicrm-sql-connect':
      return dt('A string which connects to the civicrm database.');

    case 'drush:civicrm-sql-cli':
      return dt('Quickly enter the mysql command line.');

    case 'drush:civicrm-sql-dump':
      return dt('Prints the whole CiviCRM database to STDOUT or save to a file.');

    case 'drush:civicrm-sql-query':
      return dt("Usage: drush [options] civicrm-sql-query <query>...\n<query> is a SQL statement. Any additional arguments are passed to the mysql command directly.");
  }
}

/**
 * Implementation of command 'civicrm-ext-list'
 */
function drush_civicrm_ext_list() {
  if (!civicrm_initialize()) {
    drush_print(dt("CiviCRM is not setup."));
    return;
  }
  $status = drush_get_option('status', FALSE);
  $params = [
    'options' => [
      'limit' => 0,
    ],
  ];
  if ($status) {
    if (!in_array($status, ['installed', 'uninstalled', 'disabled'])) {
      return drush_set_error('CIVICRM_INVALID_STATUS', dt("Extension status {$status} is invalid."));
    }
    $params['status'] = $status;
  }

  try {
    $result = civicrm_api3('extension', 'get', $params);
    $rows = [[dt('App name'), dt('Status'), dt('Version')]];
    foreach ($result['values'] as $k => $extension_data) {
      $rows[] = [
        $extension_data['key'],
        $extension_data['status'],
        $extension_data['version'],
      ];
    }
    unset($result);

    $format = drush_get_option('out', 'pretty');
    switch ($format) {
      case 'pretty':
        drush_print_table($rows, TRUE);
        break;

      case 'json':
        drush_print(json_encode($rows));
        break;

      default:
        return drush_set_error('CIVICRM_UNKNOWN_FORMAT', dt('Unknown format: @format', array('@format' => $format)));
    }
  }
  catch (CiviCRM_API3_Exception $e) {
    // handle error here
    $errorMessage = $e->getMessage();
    $errorCode = $e->getErrorCode();
    $errorData = $e->getExtraParams();
    drush_log(dt("!error", ['!error' => $errorData], 'error'));
  }
}

/**
 * Implementation of command 'civicrm-ext-install'
 */
function drush_civicrm_ext_install($extension_name) {
  if (!civicrm_initialize()) {
    drush_print(dt('CiviCRM is not setup.'));
    return;
  }
  try {
    $result = civicrm_api3('extension', 'install', ['key' => $extension_name]);
    if ($result['values'] && $result['values'] == 1) {
      drush_print(dt('Extension !ename installed.', ['!ename' => $extension_name]));
    }
    else {
      drush_log(t('Extension !ename could not be installed.', ['!ename' => $extension_name]), 'error');
    }
  }
  catch (CiviCRM_API3_Exception $e) {
    // handle error here
    $errorMessage = $e->getMessage();
    $errorCode = $e->getErrorCode();
    $errorData = $e->getExtraParams();
    drush_log(dt('!error', ['!error' => $errorData], 'error'));
  }
}

/**
 * Implementation of command 'civicrm-ext-disable'
 */
function drush_civicrm_ext_disable($extension_name) {
  if (!civicrm_initialize()) {
    drush_print(dt("CiviCRM is not setup."));
    return;
  }
  try {
    $result = civicrm_api3('extension', 'disable', ['key' => $extension_name]);
    if ($result['values'] && $result['values'] == 1) {
      drush_print(dt("Extension !ename disabled.", ['!ename' => $extension_name]));
    }
    else {
      drush_log(t('Extension !ename could not be disabled.', ['!ename' => $extension_name]), 'error');
    }
  }
  catch (CiviCRM_API3_Exception $e) {
    // handle error here
    $errorMessage = $e->getMessage();
    $errorCode = $e->getErrorCode();
    $errorData = $e->getExtraParams();
    drush_log(dt("!error", ['!error' => $errorData], 'error'));
  }
}

/**
 * Implementation of command 'civicrm-ext-uninstall'
 */
function drush_civicrm_ext_uninstall($extension_name) {
  if (!civicrm_initialize()) {
    drush_print(dt("CiviCRM is not setup."));
    return;
  }
  try {
    $result = civicrm_api3('extension', 'uninstall', ['key' => $extension_name]);
    if ($result['values'] && $result['values'] == 1) {
      drush_print(dt('Extension !ename uninstalled.', ['!ename' => $extension_name]));
    }
    else {
      drush_log(t('Extension !ename could not be uninstalled.', ['!ename' => $extension_name]), 'error');
    }
  }
  catch (CiviCRM_API3_Exception $e) {
    // handle error here
    $errorMessage = $e->getMessage();
    $errorCode = $e->getErrorCode();
    $errorData = $e->getExtraParams();
    drush_log(dt("!error", ['!error' => $errorData], 'error'));
  }
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-upgrade-db().
 */
function drush_civicrm_upgrade_db_validate() {
  if (!defined('CIVICRM_UPGRADE_ACTIVE')) {
    define('CIVICRM_UPGRADE_ACTIVE', 1);
  }
  $_GET['q'] = 'civicrm/upgrade';

  if (!_civicrm_init()) {
    return FALSE;
  }

  $_POST['upgrade'] = 1;
  $_GET['q'] = 'civicrm/upgrade';
  require_once 'CRM/Core/Config.php';

  require_once 'CRM/Utils/System.php';
  require_once 'CRM/Core/BAO/Domain.php';
  $codeVer = CRM_Utils_System::version();
  $dbVer = CRM_Core_BAO_Domain::version();
  if (!$dbVer) {
    return drush_set_error('CIVICRM_VERSION_MISSING_DATABASE', dt('Version information missing in civicrm database.'));
  }
  elseif (stripos($dbVer, 'upgrade')) {
    return drush_set_error('CIVICRM_DATABASE_CHECK_FAIL', dt('Database check failed - the database looks to have been partially upgraded. You may want to reload the database with the backup and try the upgrade process again.'));
  }
  elseif (!$codeVer) {
    return drush_set_error('CIVICRM_VERSION_MISSING_CODE', dt('Version information missing in civicrm codebase.'));
  }
  elseif (version_compare($codeVer, $dbVer) > 0) {
    drush_log(dt("Starting with v!dbVer -> v!codeVer upgrade ..", ['!dbVer' => $dbVer, '!codeVer' => $codeVer]));
  }
  elseif (version_compare($codeVer, $dbVer) < 0) {
    return drush_set_error('CIVICRM_VERSION_UNEXPECTED', dt("Database is marked with an unexpected version '!dbVer' which is higher than that of codebase version '!codeVer'.", [
      '!dbVer' => $dbVer,
      '!codeVer' => $codeVer,
    ]));
  }

  return TRUE;
}

/**
 * Implementation of command 'civicrm-upgrade-db'
 *
 * @throws \CRM_Core_Exception
 */
function drush_civicrm_upgrade_db() {
  $codeVer = CRM_Utils_System::version();
  $dbVer = CRM_Core_BAO_Domain::version();
  if (version_compare($codeVer, $dbVer) == 0) {
    drush_print(dt('You are already upgraded to CiviCRM @version', ['@version' => $codeVer]));
    return TRUE;
  }
  $upgradeHeadless = new CRM_Upgrade_Headless();
  // FIXME Exception handling?
  $result = $upgradeHeadless->run();
  drush_print("Upgrade outputs:\n" . $result['text']);
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-update-cfg().
 */
function drush_civicrm_update_cfg_validate() {
  return _civicrm_init();
}

/**
 * Implementation of command 'civicrm-update-cfg'
 */
function drush_civicrm_update_cfg() {
  $defaultValues = [];
  $states = ['old', 'new'];
  for ($i = 1; $i <= 3; $i++) {
    foreach ($states as $state) {
      $name = "{$state}Val_{$i}";
      $value = drush_get_option($name, NULL);
      if ($value) {
        $defaultValues[$name] = $value;
      }
    }
  }

  require_once 'CRM/Core/I18n.php';
  require_once 'CRM/Core/BAO/ConfigSetting.php';
  $result = CRM_Core_BAO_ConfigSetting::doSiteMove($defaultValues);

  if ($result) {

    drush_log(dt('Config successfully updated.'), 'completed');

  }
  else {
    drush_log(dt('Config update failed.'), 'failed');
  }
}

/**
 * Implements hook_drush_cache_clear().
 */
function civicrm_drush_cache_clear(&$types) {
  if (_civicrm_init(FALSE)) {
    $types['civicrm'] = 'drush_civicrm_cacheclear';
  }
}

/**
 * Cache clear callback
 *
 * Warning: do not name drush_civicrm_cache_clear() otherwise it will
 * conflict with hook_drush_cache_clear() and be called systematically
 * when "drush cc" is called.
 */
function drush_civicrm_cacheclear() {
  _civicrm_init();

  // Clear the classloader cache variable
  // Should be done in CiviCRM core so that the system flush always deletes
  // the variable, however, it needs to be done early enough before the
  // ClassLoader initialization. FIXME.
  variable_del('civicrm_class_loader');

  // Flush all caches using the API
  $params = ['version' => 3];
  if (drush_get_option('triggers', FALSE)) {
    $params['triggers'] = 1;
  }

  if (drush_get_option('sessions', FALSE)) {
    $params['session'] = 1;
  }

  try {
    $result = civicrm_api3('System', 'flush', $params);
  }
  catch (CiviCRM_API3_Exception $e) {
    drush_log(dt('An error occurred: !message', ['!message' => $e->getMessage()]), 'error');
    return;
  }

  drush_log(dt('The CiviCRM cache has been cleared.'), 'ok');
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-enable-debug().
 */
function drush_civicrm_enable_debug_validate() {
  return _civicrm_init();
}

function drush_civicrm_enable_debug() {
  $settings = [
    'debug_enabled' => 1,
    'backtrace' => 1,
  ];

  foreach ($settings as $key => $val) {
    try {
      $result = civicrm_api3('Setting', 'create', [$key => $val]);
    }
    catch (CiviCRM_API3_Exception $e) {
      drush_log(dt('An error occurred: !message', ['!message' => $e->getMessage()]), 'error');
      return;
    }
  }

  drush_log(dt('CiviCRM debug setting enabled.'), 'ok');
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-disable-debug().
 */
function drush_civicrm_disable_debug_validate() {
  return _civicrm_init();
}

function drush_civicrm_disable_debug() {
  $settings = [
    'debug_enabled' => 0,
    'backtrace' => 0,
  ];

  foreach ($settings as $key => $val) {
    try {
      $result = civicrm_api3('Setting', 'create', [$key => $val]);
    }
    catch (CiviCRM_API3_Exception $e) {
      drush_log(dt('An error occurred: !message', ['!message' => $e->getMessage()]), 'error');
      return;
    }
  }

  drush_log(dt('CiviCRM debug setting disabled.'), 'ok');
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-upgrade().
 */
function drush_civicrm_upgrade_validate() {
  // TODO: use Drush to download tarfile.
  // TODO: if tarfile is not specified, see if the code already exists and use that instead.
  $tarfile = drush_get_option('tarfile', FALSE);
  if (!$tarfile) {
    return drush_set_error('CIVICRM_TAR_NOT_SPECIFIED', dt('Tarfile not specified.'));
  }
  //FIXME: throw error if tarfile is not in a valid format.

  if (!defined('CIVICRM_UPGRADE_ACTIVE')) {
    define('CIVICRM_UPGRADE_ACTIVE', 1);
  }
  return _civicrm_init();
}

/**
 * Implementation of command 'civicrm-upgrade'
 */
function drush_civicrm_upgrade() {
  global $civicrm_root;

  $tarfile = drush_get_option('tarfile', FALSE);
  $date = date('YmdHis');
  $backup_file = "civicrm";

  $basepath = explode('/', $civicrm_root);
  array_pop($basepath);
  $project_path = implode('/', $basepath) . '/';

  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $backup_dir = drush_get_option('backup-dir', $drupal_root . '/../backup');
  $backup_dir = rtrim($backup_dir, '/');

  drush_print(dt("\nThe upgrade process involves - "));
  drush_print(dt("1. Backing up current CiviCRM code as => !path",
    ['!path' => "$backup_dir/modules/$date/$backup_file"]
  ));
  drush_print(dt("2. Backing up database as => !path",
    ['!path' => "$backup_dir/modules/$date/$backup_file.sql"]
  ));
  drush_print(dt("3. Unpacking tarfile to => !path",
    ['!path' => "$project_path"]
  ));
  drush_print(dt("4. Executing civicrm/upgrade?reset=1 just as a browser would.\n"));
  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_user_abort();
  }

  @drush_op('mkdir', $backup_dir, 0777);
  $backup_dir .= '/modules';
  @drush_op('mkdir', $backup_dir, 0777);
  $backup_dir .= "/$date";
  @drush_op('mkdir', $backup_dir, 0777);
  $backup_target = $backup_dir . '/' . $backup_file;
  if (!drush_op('rename', $civicrm_root, $backup_target)) {
    return drush_set_error('CIVICRM_BACKUP_FAILED', dt('Failed to backup CiviCRM project directory !source to !backup_target',
      ['!source' => $civicrm_root, '!backup_target' => $backup_target]
    ));
  }
  drush_log(dt("\n1. Code backed up."), 'ok');

  drush_set_option('result-file', $backup_target . '.sql');
  drush_civicrm_sqldump();
  drush_log(dt('2. Database backed up.'), 'ok');

  // Decompress & Untar
  _civicrm_extract_tarfile($project_path);
  drush_log(dt('3. Tarfile unpacked.'), 'ok');

  drush_log(dt("4. "));

  if (drush_civicrm_upgrade_db_validate()) {
    drush_civicrm_upgrade_db();
  }
  drush_log(dt("\nProcess completed."), 'completed');
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-restore().
 */
function drush_civicrm_restore_validate() {
  _civicrm_dsn_init();

  $restore_dir = drush_get_option('restore-dir', FALSE);
  $restore_dir = rtrim($restore_dir, '/');
  if (!$restore_dir) {
    return drush_set_error('CIVICRM_RESTORE_NOT_SPECIFIED', dt('Restore-dir not specified.'));
  }
  $sql_file = $restore_dir . '/civicrm.sql';
  if (!file_exists($sql_file)) {
    return drush_set_error('CIVICRM_RESTORE_CIVICRM_SQL_NOT_FOUND', dt('Could not locate civicrm.sql file in the restore directory.'));
  }
  $code_dir = $restore_dir . '/civicrm';
  if (!is_dir($code_dir)) {
    return drush_set_error('CIVICRM_RESTORE_DIR_NOT_FOUND', dt('Could not locate civicrm directory inside restore-dir.'));
  }
  elseif (!file_exists("$code_dir/civicrm-version.php")) {
    return drush_set_error('CIVICRM_RESTORE_DIR_NOT_VALID', dt('civicrm directory inside restore-dir, doesn\'t look to be a valid civicrm codebase.'));
  }

  return TRUE;
}

/**
 * Implementation of command 'civicrm-restore'
 */
function drush_civicrm_restore() {
  $restore_dir = drush_get_option('restore-dir', FALSE);
  $restore_dir = rtrim($restore_dir, '/');
  $sql_file = $restore_dir . '/civicrm.sql';
  $code_dir = $restore_dir . '/civicrm';
  $date = date('YmdHis');

  global $civicrm_root;
  $civicrm_root_base = explode('/', $civicrm_root);
  array_pop($civicrm_root_base);
  $civicrm_root_base = implode('/', $civicrm_root_base) . '/';

  $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $restore_backup_dir = drush_get_option('backup-dir', $drupal_root . '/../backup');
  $restore_backup_dir = rtrim($restore_backup_dir, '/');

  // get confirmation from user -
  $db_spec = drush_civicrm_get_db_spec();
  drush_print(dt("\nProcess involves :"));
  drush_print(dt("1. Restoring '\$restore-dir/civicrm' directory to '!toDir'.", ['!toDir' => $civicrm_root_base]));
  drush_print(dt("2. Dropping and creating '!db' database.", ['!db' => $db_spec['database']]));
  drush_print(dt("3. Loading '\$restore-dir/civicrm.sql' file into the database."));
  drush_print();
  drush_print(dt("Note: Before restoring a backup will be taken in '!path' directory.",
    ['!path' => "$restore_backup_dir/modules/restore"]
  ));
  drush_print();
  if (!drush_confirm(dt('Do you really want to continue?'))) {
    return drush_user_abort();
  }

  // create restore-backup-dir if not already exists
  @drush_op('mkdir', $restore_backup_dir, 0777);
  $restore_backup_dir .= '/modules';
  @drush_op('mkdir', $restore_backup_dir, 0777);
  $restore_backup_dir .= '/restore';
  @drush_op('mkdir', $restore_backup_dir, 0777);
  $restore_backup_dir .= "/$date";
  @drush_op('mkdir', $restore_backup_dir, 0777);

  // 1. backup and restore codebase
  drush_log(dt('Restoring civicrm codebase ..'));
  if (is_dir($civicrm_root) && !drush_op('rename', $civicrm_root, $restore_backup_dir . '/civicrm')) {
    return drush_set_error('CIVICRM_RESTORE_CODE_FAILED', dt("Failed to take backup for '!destination' directory",
      ['!destination' => $civicrm_root]
    ));
  }
  if (!drush_op('rename', $code_dir, $civicrm_root)) {
    return drush_set_error('CIVICRM_RESTORE_DESTINATION_FAILED', dt("Failed to restore civicrm directory '!source' to '!dest'",
      ['!source' => $code_dir, '!dest' => $civicrm_root_base]
    ));
  }
  drush_log(dt('Codebase restored.'), 'ok');

  // 2. backup, drop and create database
  drush_set_option('result-file', $restore_backup_dir . '/civicrm.sql');
  drush_civicrm_sqldump();

  drush_log(dt('Database backed up.'), 'ok');

  if (version_compare(DRUSH_VERSION, 7, '>=')) {
    $sql = drush_sql_get_class();
    $exec = 'mysql ' . $sql->creds() . ' -e ';
    $dbDriver = $db_spec['driver'];
  }
  else {
    $exec = 'mysql' . _drush_sql_get_credentials() . ' -e ';
    $dbDriver = _drush_sql_get_scheme();
  }
  drush_log(dt("\nDropping database '!db' ..", ['!db' => $db_spec['database']]));
  if (drush_op('system', $exec . '"DROP DATABASE IF EXISTS ' . $db_spec['database'] . '"')) {
    return drush_set_error('CIVICRM_RESTORE_DATABASE_DROP_FAILED', dt('Could not drop database: @name', ['@name' => $db_spec['database']]));
  }
  drush_log(dt('Database dropped.'), 'ok');
  $exec = str_replace($db_spec['database'], '', $exec);
  if (drush_op('system', $exec . '"CREATE DATABASE ' . $db_spec['database'] . '"')) {
    return drush_set_error('CIVICRM_RESTORE_DATABASE_CREATE_FAILED', dt('Could not create new database: @name', ['@name' => $db_spec['database']]));
  }
  drush_log(dt('Database created.'), 'ok');

  // 3. restore database
  switch ($dbDriver) {
    case 'mysql':
      if (version_compare(DRUSH_VERSION, 7, '>=')) {
        $send = 'mysql ' . $sql->creds();
      }
      else {
        $send = 'mysql' . _drush_sql_get_credentials();
      }
      break;

    case 'pgsql':
      if (version_compare(DRUSH_VERSION, 7, '>=')) {
        $send .= 'psql -d ' . $sql->creds() . ' --file -';
      }
      else {
        $send .= 'psql -d ' . _drush_sql_get_credentials() . ' --file -';
      }
      break;
  }
  $exec = "$send < $sql_file";
  drush_log(dt('Loading civicrm.sql file from restore-dir ..'));
  drush_op('system', $exec);
  drush_log(dt('Database restored.'), 'ok');

  drush_log(dt('Restore process completed.'), 'completed');

  _civicrm_dsn_close();
}

/**
 * Implementation of command 'civicrm-member-records'
 */
function drush_civicrm_member_records() {
  _civicrm_init();

  $_REQUEST['name'] = drush_get_option('civicrm_cron_username', NULL);
  $_REQUEST['pass'] = drush_get_option('civicrm_cron_password', NULL);
  $_REQUEST['key'] = drush_get_option('civicrm_sitekey', NULL);

  global $argv;
  $argv = [
    0 => "drush",
    1 => "-u" . $_REQUEST['name'],
    2 => "-p" . $_REQUEST['pass'],
    3 => "-s" . drush_get_option('uri', FALSE),
  ];

  if (!defined('CIVICRM_CONFDIR')) {
    define('CIVICRM_CONFDIR', drush_get_context('DRUSH_DRUPAL_ROOT') . '/sites/');
  }

  include "bin/UpdateMembershipRecord.php";
}

/**
 * Implements drush_hook_COMMAND_validate() for civicrm-rest().
 */
function drush_civicrm_rest_validate() {
  $query = drush_get_option('query', FALSE);
  if (!$query) {
    drush_set_error('CIVICRM_REST_EMPTY_QUERY', dt('query not specified.'));
  }

  return _civicrm_init();
}

/**
 * Implementation of command 'civicrm-rest' ('cvr')
 */
function drush_civicrm_rest() {
  $query = drush_get_option('query', FALSE);
  $query = explode('&', $query);
  $_GET['q'] = array_shift($query);
  foreach ($query as $keyVal) {
    list($key, $val) = explode('=', $keyVal);
    $_REQUEST[$key] = $val;
    $_GET[$key] = $val;
  }

  require_once 'CRM/Utils/REST.php';
  $rest = new CRM_Utils_REST();

  require_once 'CRM/Core/Config.php';
  $config = CRM_Core_Config::singleton();

  global $civicrm_root;
  // adding dummy script, since based on this api file path is computed.
  $_SERVER['SCRIPT_FILENAME'] = "$civicrm_root/extern/rest.php";

  if (isset($_GET['json']) &&
    $_GET['json']
  ) {
    header('Content-Type: text/javascript');
  }
  else {
    header('Content-Type: text/xml');
  }
  echo $rest->run($config);
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_pre_civicrm_sql_dump() {
  _civicrm_dsn_init();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_pre_civicrm_sqldump() {
  _civicrm_dsn_init();
}

/**
 * Implementation of command 'civicrm-sql-dump'
 */
function drush_civicrm_sqldump() {
  $extra_options = drush_get_option('extra-options', '');
  drush_set_option('extra', '--routines ' . $extra_options);
  if (version_compare(DRUSH_VERSION, 7, '>=')) {
    drush_sql_dump();
  }
  else {
    drush_sql_dump_execute();
  }
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_post_civicrm_sql_dump() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_post_civicrm_sqldump() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_pre_civicrm_sql_conf() {
  _civicrm_dsn_init();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_pre_civicrm_sqlconf() {
  _civicrm_dsn_init();
}

/**
 * Implementation of command 'civicrm-sql-conf'
 */
function drush_civicrm_sqlconf() {
  $conf = drush_sql_conf();
  // Before drush 6 drush_sql_conf already does drush_print_r, so it shouldn't
  // be called again.
  if (version_compare(DRUSH_VERSION, 6, '>=')) {
    drush_print_r($conf);
  }

  // Return the conf array too, so it can be used as array when called through
  // a php function (e.g. drush_invoke_process).
  return $conf;
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_post_civicrm_sql_conf() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_post_civicrm_sqlconf() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_pre_civicrm_sql_connect() {
  _civicrm_dsn_init();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_pre_civicrm_sqlconnect() {
  _civicrm_dsn_init();
}

/**
 * Implementation of command 'civicrm-sql-connect'
 */
function drush_civicrm_sqlconnect() {
  return drush_sql_connect();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_post_civicrm_sql_connect() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_post_civicrm_sqlconnect() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_pre_civicrm_sql_query() {
  _civicrm_dsn_init();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_pre_civicrm_sqlquery() {
  _civicrm_dsn_init();
}

/**
 * Implementation of command 'civicrm-sql-query'
 */
function drush_civicrm_sqlquery($query = '') {
  return drush_sql_query($query);
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_post_civicrm_sql_query() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_post_civicrm_sqlquery() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_pre_civicrm_sql_cli() {
  _civicrm_dsn_init();
}

/**
 * Implements drush_hook_pre_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_pre_civicrm_sqlcli() {
  _civicrm_dsn_init();
}

/**
 * Implementation of command 'civicrm-sql-cli'
 */
function drush_civicrm_sqlcli() {
  drush_sql_cli();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 6.
 */
function drush_civicrm_post_civicrm_sql_cli() {
  _civicrm_dsn_close();
}

/**
 * Implements drush_hook_post_COMMAND().
 *
 * this function is called when using drush 5.
 */
function drush_civicrm_post_civicrm_sqlcli() {
  _civicrm_dsn_close();
}

/**
 * Creates a db_url based on CIVICRM_DSN or cli_db_url setting
 */
function civicrm_db_array() {
  if (defined('CIVICRM_CLI_DSN')) {
    $url = CIVICRM_CLI_DSN;
  }
  else {
    $url = CIVICRM_DSN;
  }
  return drush_convert_db_from_db_url($url);
}

function _civicrm_dsn_init($reset = FALSE) {
  static $globalDbUrl = NULL;

  if (!_civicrm_init(TRUE, FALSE)) {
    return FALSE;
  }

  // check if we're using the old-style $GLOBALS['db_url']
  // or the new style ( > drupal 7 )
  if (drush_drupal_major_version() >= 7) {
    $database = drush_get_option('database', 'default');
    $target = drush_get_option('target', 'default');
    if (!$globalDbUrl && CIVICRM_DSN) {
      if (isset($GLOBALS['databases'][$database][$target])) {
        // keep a copy so that we can put it back.
        $globalDbUrl = $GLOBALS['databases'][$database][$target];
      }
      // now modify $GLOBALS so that drush works on CIVICRM_DSN instead of drupal's
      $GLOBALS['databases'][$database][$target] = civicrm_db_array();
    }
  }
  else {
    if (!$globalDbUrl && CIVICRM_DSN) {
      // keep a copy so that we can put it back.
      $globalDbUrl = $GLOBALS['db_url'];
    }
    // now modify $GLOBALS so that drush works on CIVICRM_DSN instead of drupal's
    $GLOBALS['db_url'] = CIVICRM_DSN;
  }
  $dbUrl = $globalDbUrl;
  $globalDbUrl = $reset ? NULL : $globalDbUrl;

  return $dbUrl;
}

function _civicrm_dsn_close() {
  $globalDbUrl = _civicrm_dsn_init(TRUE);
  if ($globalDbUrl) {
    if (drush_drupal_major_version() >= 7) {
      $database = drush_get_option('database', 'default');
      $target = drush_get_option('target', 'default');
      $GLOBALS['databases'][$database][$target] = $globalDbUrl;
    }
    else {
      $GLOBALS['db_url'] = $globalDbUrl;
    }
  }
}

/**
 * Initializes the CiviCRM environment and configuration.
 * TODO: document why we can't call civicrm_initialize() directly.
 *
 * @param bool $fail
 *   If true, will halt drush. Otherwise, return false but do not interrupt.
 * @param bool $load_config
 *   If true, loads the CiviCRM configuration.
 *
 * @return bool
 *   Returns TRUE if CiviCRM was initialized.
 */
function _civicrm_init($fail = TRUE, $load_config = TRUE) {
  static $init = NULL;

  // return if already initialized
  if ($init) {
    return $init;
  }

  if (!version_compare(phpversion(), CIVICRM_DRUPAL_PHP_MINIMUM, '>=')) {
    return drush_set_error('CIVICRM_PHP_MINIMUM', dt('CiviCRM requires PHP @required+. Drush is running PHP @current.', [
      '@current' => phpversion(),
      '@required' => CIVICRM_DRUPAL_PHP_MINIMUM,
    ]));
  }

  global $cmsPath;
  $cmsPath = $drupal_root = drush_get_context('DRUSH_DRUPAL_ROOT');
  $site_root = drush_get_context('DRUSH_DRUPAL_SITE_ROOT', FALSE);
  $civicrmSettingsFile = "$drupal_root/$site_root/civicrm.settings.php";

  if (!file_exists($civicrmSettingsFile)) {
    $sites_subdir = drush_get_option('sites-subdir', 'default');
    $civicrmSettingsFile = "$drupal_root/sites/$sites_subdir/civicrm.settings.php";
    if (!file_exists($civicrmSettingsFile)) {
      if ($fail) {
        return drush_set_error('CIVICRM_INIT_SETTINGS_NOT_FOUND', dt('Could not locate civicrm settings file.'));
      }
      else {
        return FALSE;
      }
    }
  }
  // include settings file
  define('CIVICRM_SETTINGS_PATH', $civicrmSettingsFile);
  include_once $civicrmSettingsFile;
  global $civicrm_root;
  if (!is_dir($civicrm_root)) {
    return drush_set_error('CIVICRM_INIT_CODEBASE_NOT_FOUND', dt('Could not locate CiviCRM codebase. Make sure CiviCRM settings file has correct information.'));
  }

  require_once $civicrm_root . '/CRM/Core/ClassLoader.php';
  CRM_Core_ClassLoader::singleton()->register();

  if ($load_config) {
    require_once 'CRM/Core/Config.php';
    CRM_Core_Config::singleton();
  }

  $init = TRUE;
  return $init;
}

function _civicrm_get_crmpath() {
  if (!$crmpath = drush_get_option('destination', FALSE)) {
    $crmpath = drush_get_context('DRUSH_DRUPAL_SITE_ROOT', FALSE) . '/modules/';
    if (!is_dir($crmpath)) {
      $crmpath = "sites/all/modules";
    }
  }
  return $crmpath;
}

/**
 * (Drush callback)
 *
 * Implementation of command 'civicrm-api'
 */
function drush_civicrm_api() {
  $DEFAULTS = ['version' => 3];

  $args = func_get_args();
  list($entity, $action) = explode('.', $args[0]);
  array_shift($args);

  // Parse $params
  $format = drush_get_option('in', 'args');
  switch ($format) {
    case 'args':
      $params = $DEFAULTS;
      foreach ($args as $arg) {
        preg_match('/^([^=]+)=(.*)$/', $arg, $matches);
        $params[$matches[1]] = $matches[2];
      }
      break;

    case 'json':
      $json = stream_get_contents(STDIN);
      if (empty($json)) {
        $params = $DEFAULTS;
      }
      else {
        $params = array_merge($DEFAULTS, json_decode($json, TRUE));
      }
      break;

    default:
      drush_set_error(dt('Unknown format: @format', ['@format' => $format]));
      break;
  }

  if (!civicrm_initialize()) {
    drush_print(dt("CiviCRM is not setup."));
    return;
  }

  global $user;
  CRM_Core_BAO_UFMatch::synchronize($user, FALSE, 'Drupal',
    civicrm_get_ctype('Individual')
  );

  switch ($params['version']) {
    case 3:
      try {
        unset($params['version']);
        $result = civicrm_api3($entity, $action, $params);
      }
      catch (CiviCRM_API3_Exception $e) {
        drush_set_error('CIVICRM api error', $e->getMessage());
      }
      break;

    case 4:
      try {
        unset($params['version']);
        $result = civicrm_api4($entity, $action, $params);
      }
      catch (API_Exception $e) {
        drush_set_error('CIVICRM api error', $e->getMessage());
      }
      break;

    default:
      drush_set_error(dt('Unknown api version: @version', ['@version' => $params['version']]));
      break;

  }

  $format = drush_get_option('out', 'pretty');
  switch ($format) {
    case 'pretty':
      drush_print_r($result);
      break;

    case 'json':
      drush_print(json_encode($result));
      break;

    default:
      return drush_set_error('CIVICRM_UNKNOWN_FORMAT', dt('Unknown format: @format', ['@format' => $format]));
  }
}

/**
 * (Drush callback)
 *
 * Implementation of command 'civicrm-pipe'
 */
function drush_civicrm_pipe(?string $connection_flags = NULL) {
  if (!civicrm_initialize()) {
    return drush_set_error('CIVICRM_UNINSTALLED', dt('CiviCRM is not setup.'));
  }
  if (!is_callable(['Civi', 'pipe'])) {
    return drush_set_error('CIVICRM_PIPE_UNSUPPORTED', dt('This version of CiviCRM does not include Civi::pipe() support.'));
  }
  if (!empty($connection_flags)) {
    Civi::pipe($connection_flags);
  }
  else {
    Civi::pipe();
  }
}

/**
 * Implementation of command 'civicrm-sync-users-contacts'
 */
function drush_civicrm_sync_users_contacts() {
  if (!civicrm_initialize()) {
    drush_print(dt("CiviCRM is not setup."));
    return;
  }
  $result = CRM_Utils_System::synchronizeUsers();
  $status = ts('Checked one user record.',
    [
      'count' => $result['contactCount'],
      'plural' => 'Checked %count user records.',
    ]
  );
  if ($result['contactMatching']) {
    $status .= PHP_EOL . ts('Found one matching contact record.',
        [
          'count' => $result['contactMatching'],
          'plural' => 'Found %count matching contact records.',
        ]
      );
  }

  $status .= PHP_EOL . ts('Created one new contact record.',
      [
        'count' => $result['contactCreated'],
        'plural' => 'Created %count new contact records.',
      ]
    );
  drush_print(dt($status));
  return $result;
}

function drush_civicrm_sql_rebuild_triggers() {
  $table = drush_get_option('table', NULL);
  if (!civicrm_initialize()) {
    drush_print(dt("CiviCRM is not setup."));
    return;
  }
  $triggerService = Civi::service('sql_triggers');
  $triggerService->rebuild($table, TRUE);
  if (\Civi::settings()->get('logging_no_trigger_permission')) {
    $file = $triggerService->getFile();
    drush_print("Trigger SQL written to $file");
  }
  else {
    drush_print("Triggers rebuilt");
  }
}

/**
 * Callback function for civicrm-process-mail-queue.
 *
 * Call the process_mailing job to process any outstanding mailing jobs. This
 * command will need to be run as a user with the correct permissions to
 * process mailings.
 *
 * @see civicrm_drush_command()
 */
function drush_civicrm_process_mail_queue() {
  $init = _civicrm_init();
  $facility = new CRM_Core_JobManager();
  $facility->setSingleRunParams('Job', 'process_mailing', [], 'Started by drush');
  $facility->executeJobByAction('Job', 'process_mailing');
}

/**
 * Very minimal implementation of \Psr\Log to get log messages from \Civi\Setup
 */
class CiviCRMDrushLog {

  public function emergency($message, array $context = []) {
    drush_log('CiviCRM emergency: ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'error');
  }

  public function alert($message, array $context = []) {
    drush_log('CiviCRM alert: ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'error');
  }

  public function critical($message, array $context = []) {
    drush_log('CiviCRM ' . __FUNCTION__ . ': ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'error');
  }

  public function error($message, array $context = []) {
    drush_log('CiviCRM ' . __FUNCTION__ . ': ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'error');
  }

  public function warning($message, array $context = []) {
    drush_log('CiviCRM ' . __FUNCTION__ . ': ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'warning');
  }

  public function notice($message, array $context = []) {
    drush_log('CiviCRM ' . __FUNCTION__ . ': ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'warning');
  }

  public function info($message, array $context = []) {
    drush_log('CiviCRM ' . __FUNCTION__ . ': ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'info');
  }

  public function debug($message, array $context = []) {
    drush_log('CiviCRM ' . __FUNCTION__ . ': ' . $message . (!empty($context) ? ' ' . print_r($context, 1) : ''), 'debug');
  }

  public function log($level, $message, array $context = []) {
    drush_log('CiviCRM ' . $level . ': ' . $message . ' ' . print_r($context, 1), 'info');
  }

}
