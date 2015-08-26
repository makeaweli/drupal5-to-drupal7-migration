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

/*
 * Insert nodes based on form submit resluts
 * @node_types array of node content types that were selected
 */

function gemini_migrate_d5_insert_nodes($node_types) {

  // Loop through all node types from the form
  foreach($node_types as $node_type) {
    // Get all nodes form specified type
    $nodes = gemini_migrate_d5_get_nodes_by_type($node_type, 'D5');

    // Insert each node into it's respective tables
    foreach($nodes as $n) {
      gemini_migrate_d5_insert_node($n);
      gemini_migrate_d5_insert_node_revision_body($n);
      gemini_migrate_d5_insert_node_data_body($n);

    }

  }
}

/* 
 * Insert node into Drupal 7
 */
function gemini_migrate_d5_insert_node($n) {

  // insert into {node}

  db_merge('node')
    ->key(array('nid' => $n->nid))
    ->fields(array(
      'nid' => $n->nid,
      'vid' => $n->vid,
      'type' => $n->type,
      'language' => 'und',
      'title' => $n->title,
      'uid' => $n->uid,
      'status' => $n->status,
      'created' => $n->created,
      'changed' => $n->changed,
      'comment' => $n->comment,
      'promote' => $n->promote,
      'sticky' => $n->sticky
    ))
    ->execute();

  // insert into {field_data_body}

  // insert into {field_revision_body}
}

/* 
 * Insert {node_revision_body} into Drupal 7
 * @n object Drupal 5 node object
 */ 
function gemini_migrate_d5_insert_node_revision_body($n) {

  // Get Drupal 5 node revisions
  db_set_active('legacy');

  $sql = "SELECT * from {node_revisions} WHERE nid = :nid";

  $result = db_query($sql, array(':nid' => $n->nid))->fetchAll();

  db_set_active('default');

  // Insert into {field_revision_body}
  foreach($result as $r) {
    db_merge('field_revision_body')
      ->key(array('entity_id' => $r->nid))
      ->fields(array(
        'entity_id' => $r->nid,
        'entity_type' => 'node',
        'bundle' => $n->type,
        // 'deleted' => $r->deleted,
        'revision_id' => $r->vid,
        'language' => 'und',
        // 'delta' => $r->delta,
        'body_value' => $r->body,
        'body_summary' => $r->teaser,
        'body_format' => 'full_html',
      ))
      ->execute();
  }

}

/*
 * Insert node {field_data_body} into Drupal 7
 * @n object Drupal 5 node object
 */ 

function gemini_migrate_d5_insert_node_data_body($n) {

}

