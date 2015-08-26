<?php

/* 
 * Query for all Drupal 5 node content types
 * @migration_source string Either D5 or D7
 */

function gemini_migrate_d5_get_node_types($migration_source='D7') {

  switch($migration_source) {

    case 'D5' : 
      db_set_active('legacy');

      $sql = "SELECT DISTINCT type from {node}
    order by type";

      $result = db_query($sql)->fetchCol();

      db_set_active('default');

      return $result;

      break;

    case 'D7':
    default:
      db_set_active('default');

      $sql = "SELECT type from {node_type} order by type";

      $result = db_query($sql)->fetchCol();

      return $result;

      break;
    }
}

/* 
 * Query for all instances of node by type
 * @node_type string Drupal content type
 * @migration_source string Either D5 or D7
 */
function gemini_migrate_d5_get_nodes_by_type($node_type, $migration_source) {

  switch($migration_source) {

    case 'D5' : 
      db_set_active('legacy');
      break;

    case 'D7':
    default:
      db_set_active('default');
      break;
  }

  $sql = "SELECT * FROM {node} WHERE type = :node_type";

  $result = db_query($sql, array(':node_type' => $node_type))->fetchAll();

  db_set_active('default');

  return $result;
}

/* 
 * Return array for content type display as table
 */

function gemini_migrate_d5_get_node_type_table() {

  // Get node types
  $node_types = gemini_migrate_d5_get_node_types('D5');

  $options = array();
  foreach ($node_types as $node_type) {

    // get legacy record count of node type
    $source_total = count(gemini_migrate_d5_get_nodes_by_type($node_type, 'D5'));

    //  Check if content type exists and return total, if not return  message
    $destination_total = (in_array($node_type, gemini_migrate_d5_get_node_types())) ? 
    count(gemini_migrate_d5_get_nodes_by_type($node_type, 'D7')) : l('No matching content type', 'admin/structure/types/add');

    $options[$node_type] = array(
      'node_type' => $node_type,
      'source_total' => $source_total,
      'destination_total' => $destination_total,
      'rollback' => '<a href="#?">Rollback</a>'
    );
  }

  return $options;

}