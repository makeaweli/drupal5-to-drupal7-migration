<?

/* 
 * Return array for content type display as table
 */

function gemini_migrate_d5_get_form_user_table() {


  $options = array();

  $options['users'] = array(
    'entity' => 'Users',
    'source_total' => count(gemini_migrate_d5_get_users('D5')),
    'destination_total' => count(gemini_migrate_d5_get_users('D7')),
    'rollback' => '<a href="#?">Rollback</a>'
  );

  $options['roles'] = array(
    'entity' => 'Roles',
    'source_total' => count(gemini_migrate_d5_get_role('D5')),
    'destination_total' => count(gemini_migrate_d5_get_role('D7')),
    'rollback' => '<a href="#?">Rollback</a>'
  );

  $options['users_roles'] = array(
    'entity' => 'Users Roles',
    'source_total' => count(gemini_migrate_d5_get_users_roles('D5')),
    'destination_total' => count(gemini_migrate_d5_get_users_roles('D7')),
    'rollback' => '<a href="#?">Rollback</a>'
  );

  return $options;

}

/*
 * Query for all Drupal {users}
 * @migration_source string Values either D5 or D7
 * @return array SQL objects
 */
function gemini_migrate_d5_get_users($migration_source='D7') {

  switch($migration_source) {

    case 'D5': 
      db_set_active('legacy');
      break;

    case 'D7':
    default:
      db_set_active('default');
      break;
  }

  $sql = "SELECT * FROM {users}";

  $result = db_query($sql)->fetchAll();

  db_set_active('default');

  return $result;
}

/* 
 * Insert users
 * @results array Form submit data
 */
function gemini_migrate_d5_insert_users() {
  
  // Get all users from Drupal 5
  $users = gemini_migrate_d5_get_users('D5');

  // Insert users into Drupal 7
  foreach($users as $u) {
    // drupal_set_message(print_r($u, 1));
    // Convert user passwords
    // admin $S$D92ynbOZ0kOTht3EhTqrkPMuIi81E38Bt/ARvc565yYJmrqDdAgP
    // admin 2 $S$DbvkgrMpc4KiBQaebZnc9dpzVkEDXqZ9F1b6R3jiy/VEfVmPh8Xq
  //           U$S$9urM.QplzvxctfCEK86EM2MN2OMKaczV/SV9X1cp3PoSf6EGc64c

    $password = gemini_migrate_d5_convert_password($u->pass);

    // Convert timezone UTC offset from Drupal 5
    $timezone = ($u->timezone == '-36000' ? 'Pacific/Honolulu' : '');

    // Insert into Drupal 7
    db_merge('users')
      ->key(array('uid' => $u->uid))
      ->fields(array(
        'uid' => $u->uid,
        'name' => $u->name,
        'pass' => $password,
        'mail' => $u->mail,
        'theme' => $u->theme,
        'signature' => $u->signature,
        'created' => $u->created,
        'access' => $u->access,
        'login' => $u->login,
        'status' => $u->status,
        'timezone' => $timezone,
        'language' => $u->language,
        // 'picture' => $u->picture,
        'init' => $u->init,
        'data' => $u->data,
        ))
      ->execute();
  }

  drupal_set_message('{users} table migrated');

}

/*
 * Convert Drupal 5 password to Drupal 7
 * http://stackoverflow.com/questions/6205605/drupal-6-user-password-import-to-drupal-7
 * @pass string Password
 * @return string Drupal 7 converted password
 */
function gemini_migrate_d5_convert_password($password) {
  require_once DRUPAL_ROOT . '/' . variable_get('password_inc', 'includes/password.inc');
  $old_hash = $password;
  $hash_count_log2 = 11;

  $new_hash = user_hash_password($old_hash, $hash_count_log2);

  if ($new_hash) {
    // Indicate an updated password.
    $new_hash  = 'U' . $new_hash;
  }

  return $new_hash;
}

/*
 * Query for all Drupal {role}
 * @migration_source string Values either D5 or D7
 * @return array SQL objects
 */
function gemini_migrate_d5_get_role($migration_source='D7') {

  switch($migration_source) {

    case 'D5': 
      db_set_active('legacy');
      break;

    case 'D7':
    default:
      db_set_active('default');
      break;
  }

  $sql = "SELECT * FROM {role}";

  $result = db_query($sql)->fetchAll();

  db_set_active('default');

  return $result;
}


/*
 * Insert Drupal 7 {role}
 */
function gemini_migrate_d5_insert_role() {

  // Get Drupal 5 role objects
  $roles = gemini_migrate_d5_get_role('D5');

  // Insert into Drupal 7
  foreach($roles as $r) {
    db_merge('role')
      ->key(array(
          'rid' => $r->rid
        ))
      ->fields(array(
          'rid' => $r->rid,
          'name' => $r->name,
          'weight' => '1'
        ))
      ->execute();
  }

  drupal_set_message('{role} table migrated');
}

/*
 * Query for all Drupal {users_roles}
 * @migration_source string Values either D5 or D7
 * @return array SQL objects
 */
function gemini_migrate_d5_get_users_roles($migration_source='D7') {

  switch($migration_source) {

    case 'D5': 
      db_set_active('legacy');
      break;

    case 'D7':
    default:
      db_set_active('default');
      break;
  }

  $sql = "SELECT * FROM {users_roles}";

  $result = db_query($sql)->fetchAll();

  db_set_active('default');

  return $result;
}

/*
 * Insert Drupal 7 {users_roles}
 */
function gemini_migrate_d5_insert_users_roles() {

  // Get Drupal 5 role objects
  $users_roles = gemini_migrate_d5_get_users_roles('D5');

  // Insert into Drupal 7
  foreach($users_roles as $u) {
    db_merge('users_roles')
      ->key(array(
          'uid' => $u->uid,
          'rid' => $u->rid
        ))
      ->fields(array(
          'uid' => $u->uid,
          'rid' => $u->rid,
        ))
      ->execute();
  }

  drupal_set_message('{users_roles} table migrated');
}
