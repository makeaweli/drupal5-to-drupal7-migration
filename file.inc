<?php

/**
 * @param  array  $form_value Values from form table
 *                name of content_type/node_type
 */
function gemini_migrate_d5_insert_file($form_value) {

  // Get Drupal 5 {files}
  $D5_files = gemini_migrate_d5_get_files('D5');

  // Get Drupal 5 {file_revisions}
  $file_revisions = gemini_migrate_d5_get_file_revisions();

  // Get Drupal 5 {node_revisions}
  $node_revisions = gemini_migrate_d5_get_node_revisions();
  
  foreach($form_value as $content_type) {
    // Get files for current content_type
    $files = array_filter($D5_files, array(new GeminiMigrateD5UtilityFile($content_type, 'node_type'), 'isEqual'));

    $file_revisions_published = gemini_migrate_d5_get_file_revisions_published($content_type);

    foreach($files as $f) {

      /**
       * Insert into {file_managed}
       */
      
      // ** Get uid from revisions
      // get vid from file_revisions
      $fr = array_filter($file_revisions, array(new GeminiMigrateD5UtilityFile($f['fid'], 'fid'), 'isEqual'));
      $vids = arrayColumn($fr, 'vid');
      $vid = $vids[0];
      
      // get uid from node_revisions
      $nr = array_filter($node_revisions, array(new GeminiMigrateD5UtilityFile($vid, 'vid'), 'isEqual'));
      // reindex array
      $nr = array_values($nr);
      $uid = (isset($nr[0]['uid'])) ? $nr[0]['uid'] : 0;
      $timestamp = (isset($nr[0]['timestamp'])) ? $nr[0]['timestamp'] : 0;

      db_merge('file_managed')
        ->key(array('fid' => $f['fid']))
        ->fields(array(
                'fid' => $f['fid'],
                'uid' => $uid,
                'filename' => $f['filename'],
                'uri' => substr_replace($f['filepath'], 'public://', 0,6),
                'filemime' => $f['filemime'],
                'filesize' => $f['filesize'],
                'status' => '1',
                'timestamp' => $timestamp,
                'origname' => $f['filename'],
          ))
        ->execute();

      /**
       * Insert into {field_revision_field_file}
       */
      foreach($fr as $r) {

        db_merge('field_revision_field_file')
          ->key(array('revision_id' => $r['vid']))
          ->fields(array(
              'entity_type' => 'node',
              'bundle' => $content_type,
              'deleted' => '0',
              'entity_id' => $f['nid'],
              'revision_id' => $r['vid'],
              'language' => 'und',
              'delta' => '0',
              'field_file_fid' => $f['fid'],
              'field_file_display' => $r['list'],
              'field_file_description' => $r['description'],
          ))
          ->execute();

      } 

    } // end {files} loop

    foreach($file_revisions_published as $k=>$frp) {

      db_merge('field_data_field_file')
        ->key(array('field_file_fid' => $frp['fid']))
        ->fields(array(
            'entity_type' => 'node',
            'bundle' => $content_type,
            'deleted' => '0',
            'entity_id' => $frp['nid'],
            'revision_id' => $frp['vid'],
            'language' => 'und',
            'delta' => $k,
            // 'field_file_fid' => $frp['fid'],
            'field_file_display' => $frp['list'],
            'field_file_description' => $frp['description'],
        ))
        ->execute();
    } 

      // Insert into {file_usage}
    
  } // end form content type loop

  return $files;
}

/*
 * Return array for form tabe
 */
function gemini_migrate_d5_get_file_type_table() {

  $options = array();

  // Get Drupal 5 {files}
  $files = gemini_migrate_d5_get_files('D5');

  // Get Drupal 7 {field_data_field_file}
  $field_data_field_file = gemini_migrate_d5_get_files('D7');

  foreach( array_unique(arrayColumn($files, 'node_type')) as $f ) {

    $options[$f] = array(
      'content_type' => $f,
      'source_total' => count(array_keys(arrayColumn($files, 'node_type'), $f)),
      'destination_total' => count(array_keys(arrayColumn($field_data_field_file, 'bundle'), $f)),
    );
  }

  return $options;

}

/**
 * @param  string  D5 OR D7 
 * @return array  database records
 */
function gemini_migrate_d5_get_files($migration_source='D7') {
  switch ($migration_source) {
    case 'D5':
      db_set_active('legacy');

      $sql = 'SELECT {files}.*, {node}.type as node_type FROM {files}, {node}
WHERE files.nid = node.nid';

      $result = db_query($sql)->fetchAll(PDO::FETCH_ASSOC);

      db_set_active('default');

      return $result;

      break;
    
    case 'D7':
    default:
      $sql = 'SELECT DISTINCT file_managed.*, field_revision_field_file.`bundle`
              FROM file_managed, field_revision_field_file
              WHERE file_managed.`fid` = field_revision_field_file.`field_file_fid`';

      $result = db_query($sql)->fetchAll(PDO::FETCH_ASSOC);

      return $result;
      break;
  }
}

/**
 * @return Array Drupal database records
 */
function gemini_migrate_d5_get_file_revisions() {
  db_set_active('legacy');

  $sql = "SELECT * FROM {file_revisions}";

  $result = db_query($sql)->fetchAll(PDO::FETCH_ASSOC);

  db_set_active('default');

  return $result;
}

/**
 * Get Drupal 5 {file_revisions} that are used by the currently published node revision id
 * @param  String $content_type Node content type
 * @return Array Associate array of DB records
 */
function gemini_migrate_d5_get_file_revisions_published($content_type) {
  db_set_active('legacy');

  $sql = 'SELECT {file_revisions}.*, node.nid 
          FROM {node}, {file_revisions}
          WHERE node.`vid` = file_revisions.`vid`
          AND node.type = :content_type';

  $result = db_query($sql, array(':content_type' => $content_type))->fetchAll(PDO::FETCH_ASSOC);

  db_set_active('default');

  return $result;
}

/**
 * Get Drupal 5 {node_revisions}
 * @return Array Associate array of DB records
 */
function gemini_migrate_d5_get_node_revisions() {
  db_set_active('legacy');

  $sql = 'SELECT * FROM {node_revisions}';

  $result = db_query($sql)->fetchAll(PDO::FETCH_ASSOC);

  db_set_active('default');

  return $result;
}

class GeminiMigrateD5UtilityFile {
  private $arg1;
  private $arg2;

  function __construct($arg1, $arg2=FALSE) {
    $this->arg1 = $arg1;
    $this->arg2 = $arg2;
  }

  /*
   * Return TRUE if is content type/node_type
   * @param $i array element of {url_alias}
   * @param $this->arg1 String content_type or node_type
   * @param $this->arg2 String Array key used for comparison
   * @return BOOL
   */
  function isEqual($i) {
    return $this->arg1 == $i[$this->arg2];
  }

}