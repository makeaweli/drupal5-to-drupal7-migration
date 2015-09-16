<?php

/* 
 * Insert into {url_alias}
 */
function gemini_migrate_d5_insert_url_alias($form_value) {
  // Get last insert id for Drupal 7 {url_alias}
  $last_id = gemini_migrate_d5_get_url_alias_last_insert_id();
  // Get Drupal 5 {url_alias}
  $D5_url_alias = gemini_migrate_d5_get_url_alias('D5');

  foreach($form_value as $url_alias) {
    // Create array by filtering for current $url_alias
    $D5 = array_filter($D5_url_alias, array(new GeminiMigrateD5UtilityURLAlias($url_alias, 'src'), 'isChild'));

    // Insert into Drupal 7 {url_alias}
    foreach($D5 as $ua) {
      db_merge('url_alias')
        ->key(array('source' => $ua['src']))
        ->fields(array(
            'pid' => $ua['pid'] + $last_id,
            'source' => $ua['src'],
            'alias' => $ua['dst'],
            'language' => 'und',
        ))
        ->execute();
    }
    
    // $message = sprintf('%s of %s migrated', count($D5), $url_alias);
    // drupal_set_message($message);  
  }
}

/*
 * Return array for form tabe
 */
function gemini_migrate_d5_get_url_alias_type_table() {

  $options = array();

  // Get Drupal {url_alias} items
  $D5_url_alias = gemini_migrate_d5_get_url_alias('D5');
  $D7_url_alias = gemini_migrate_d5_get_url_alias('D7');

  // Add the path root to the array
  $D5_url_alias = array_map(array(new GeminiMigrateD5UtilityURLAlias('src'), 'mapPathRoot'), $D5_url_alias);

  $D7_url_alias = array_map(array(new GeminiMigrateD5UtilityURLAlias('source'), 'mapPathRoot'), $D7_url_alias);

  foreach( array_unique(arrayColumn($D5_url_alias, 'path_root')) as $alias_type ) {

    $options[$alias_type] = array(
      'alias_type' => $alias_type,
      'source_total' => count(array_keys(arrayColumn($D5_url_alias, 'path_root'), $alias_type)),
      'destination_total' => count(array_keys(arrayColumn($D7_url_alias, 'path_root'), $alias_type)), 
    );
  }

  return $options;

}

/* 
 * Get last insert id for Drupal 7 {url_alias}
 */
function gemini_migrate_d5_get_url_alias_last_insert_id() {
  $sql = 'SELECT pid FROM {url_alias} ORDER BY pid DESC LIMIT 0,1';

  $result = db_query($sql)->fetchCol();

  if(is_array($result)) {
    return $result[0];
  }
  else{
    return FALSE;
  }
}

/*
 * Query for all Drupal {url_alias} records
 * @param $migration_source String either D5 or D7
 * @return Array
 */
function gemini_migrate_d5_get_url_alias($migration_source='D7') {

  switch($migration_source) {
    case 'D5':
      db_set_active('legacy');
      break;

    case 'D7':
    default:
      db_set_active('default');
      break;
  }

  $sql = 'SELECT * FROM {url_alias}';
  
  $result =  db_query($sql)->fetchAll(PDO::FETCH_ASSOC);

  db_set_active('default');

  return $result;

}

/*
 * Utility class to be used for callbacks
 * since we can't use closures in PHP 5.2
 */
class GeminiMigrateD5UtilityURLAlias {
  private $arg1;
  private $arg2;

  function __construct($arg1, $arg2=FALSE) {
    $this->arg1 = $arg1;
    $this->arg2 = $arg2;
  }

  /*
   * Return TRUE if is child of a URL Alias path
   * @param $i array element of {url_alias}
   * @param $this->arg1 String path root
   * @param $this->arg2 String Array key used for comparison
   * @return BOOL
   */
  function isChild($i) {
    $i = explode('/', $i[$this->arg2]);

    return $i[0] == $this->arg1;

  }

  /*
   * Modify array to include key named path_root
   * @param $i array element of {url_alias}
   * @param $this->arg1 String key for path data
   * @return array
   */
  function mapPathRoot($i) {
    $path = explode('/', $i[$this->arg1]);

    $i['path_root'] = $path[0];

    return $i;
  }
}