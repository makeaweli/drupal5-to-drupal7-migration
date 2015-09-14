<?php

function gemini_migrate_d5_get_menu($migration_source='D7') {

  switch($migration_source) {
    case 'D5':
      db_set_active('legacy');

      // WHERE necessary to prevent circular reference
      $sql = 'SELECT * FROM {menu} WHERE pid != mid';
      break;

    case 'D7':
    default:
      $sql = 'SELECT * FROM {menu_links}';
      break;
  }

  $result = db_query($sql)->fetchAll();

  db_set_active('default');

  return $result;

}

/*
 * Query for all menu types
 * @param $migration_source D5 or D7
 */
function gemini_migrate_d5_get_menu_types($migration_source = 'D7'){
  switch ($migration_source) {
    case 'D5':
      db_set_active('legacy');

      $sql = 'SELECT * FROM {menu} WHERE pid = :pid ORDER BY title ASC';

      $result = db_query($sql, array(':pid' => 0))->fetchAll();

      // Manually add Navigation menu
      // it doesn't exist in the {menu} table
      $result[] = (object) array(
           'mid' => '1',
           'pid' => '0',
           'title' => 'Navigation'
       );

      // Manually add Admin menu
      $result[] = (object) array(
        'mid' => '5',
        'pid' => '0',
        'title' => 'Navigation/Admin'
      );

      db_set_active('default');

      return $result;
      break;
    
    case 'D7':
    default:
      
      $sql = 'SELECT * FROM {menu_custom}';

      $result = db_query($sql)->fetchAll();

      return $result;
      break;
  }
}

/*
 * Check if Drupal 5 menu type has any children
 * @param $mid Drupal 5 mid
 */
function gemini_migrate_d5_menu_type_children_exists($mid) {

  db_set_active('legacy');

  $sql = 'SELECT * FROM {menu} WHERE pid = :pid';

  $result = db_query($sql, array(':pid' => $mid))->fetchAll();

  db_set_active('default');

  if(!$result) {
    return FALSE;
  }
  else {
    return $result;
  }


}

/*
 * Check if Drupal 5 menu item has any children
 * @param $mid int Drupal 5 menu ID
 */
function gemini_migrate_d5_menu_item_has_children($mid) {
  $result = gemini_migrate_d5_get_menu('D5');
  $menu = gemini_migrate_d5_menu_flat($result);

  foreach($menu as $m) {
    if($m['pid'] == $mid) {
      return TRUE;
    }
  }

  return FALSE;
}

/* 
 * Get last insert id for Drupal 7 {menu_links}
 */
function gemini_migrate_d5_get_menu_links_last_insert_id() {
  $sql = 'SELECT mlid FROM menu_links ORDER BY mlid DESC LIMIT 0,1';

  $result = db_query($sql)->fetchCol();

  if(is_array($result)) {
    return $result[0];
  }
  else{
    return FALSE;
  }
}


/*
 * Return array for form table
 */
function gemini_migrate_d5_get_menu_type_table() {

  // Get menu parents
  $menu_types = gemini_migrate_d5_get_menu_types('D5');

  // Get Drupal 7 {menu_custom}
  $menu_custom = gemini_migrate_d5_get_menu_types('D7');

  // Reduce $menu_custom into $M7
  $M7 = array();

  foreach($menu_custom as $mc) {
    $M7[] = $mc->title;
  }

  $options = array();

  foreach($menu_types as $mt){
    if(gemini_migrate_d5_menu_type_children_exists($mt->mid)) {
      $source_total = '0';

      $destination_total = (in_array($mt->title, $M7)) ? '0' : l('No matching menu type', 'admin/structure/menu/add');

      $options[$mt->title] = array(
        'menu_type' => $mt->title,
        'source_total' => $source_total,
        'destination_total' => $destination_total,
        'rollback' => 'x',
      );
    }
  }

  return $options;
}

/*
 * Flatten Drupal DB array
 * $menu Array Drupal DB object
 */
function gemini_migrate_d5_menu_flat($menu) {
  $a = array();
  foreach($menu as $m) {
    $b = array(
      'mid' => $m->mid,
      'pid' => $m->pid,
      'path' => $m->path,
      'title'=> $m->title,
      'weight' => $m->weight,
      'type' => $m->type
    );

    array_push($a, $b);
  }

  // Create entry for Navigation as it doesn't exist in the the DB
  $a[] = array(
    'mid' => '1',
    'pid' => '0',
    'path' => '',
    'title'=> 'Navigation',
    'weight' => '0',
    'type' => '118'
  );
  return $a;
}

/* 
 * http://codereview.stackexchange.com/questions/94669/adjacency-list-with-array-data
 */
function gemini_migrate_d5_menu_path($element, $input) {
  $r = $input[ array_search($element, arrayColumn($input, 'mid')) ];
  $str[] = $r['mid'];
  if($r['pid'] != 0) {
     $str[] = gemini_migrate_d5_menu_path($r['pid'], $input);
  }

  return array_flatten(array_reverse($str));

}

function arrayColumn(array $array, $column_key, $index_key=null){
        if(function_exists('array_column')){
            return array_column($array, $column_key, $index_key);
        }
        $result = array();
        foreach($array as $arr){
            if(!is_array($arr)) continue;

            if(is_null($column_key)){
                $value = $arr;
            }else{
                $value = $arr[$column_key];
            }

            if(!is_null($index_key)){
                $key = $arr[$index_key];
                $result[$key] = $value;
            }else{
                $result[] = $value;
            }

        }

        return $result;
    }

/*
 * http://stackoverflow.com/a/7858985
 */
function array_flatten(array $array)
{
    $flat = array(); // initialize return array
    $stack = array_values($array); // initialize stack
    while($stack) // process stack until done
    {
        $value = array_shift($stack);
        if (is_array($value)) // a value to further process
        {
            $stack = array_merge(array_values($value), $stack);
        }
        else // a value to take
        {
           $flat[] = $value;
        }
    }
    return $flat;
}    


/* 
 * Array of Drupal 7 {menu_custom} that map with Drupal 5
 * @return object
 */
function gemini_migrate_d5_map_menu_custom() {
  // Create array 
  $menu_custom = array();

  // Get Drupal 7 {menu_custom}
  $D7_menu_custom = gemini_migrate_d5_get_menu_types('D7');

  // Get Drupal 5 menu parents
  $D5_menu_custom = gemini_migrate_d5_get_menu_types('D5');

  foreach($D5_menu_custom as $D5) {
    // check if menu parent has any children
    if(gemini_migrate_d5_menu_type_children_exists($D5->mid)) {

      foreach($D7_menu_custom as $D7) {
        if($D5->title == $D7->title) {
          $menu_custom[] = array(
            'menu_name' => $D7->menu_name,
            'title' => $D7->title
          );
        }
      }

    }
  }

  return $menu_custom;
}
/*
 * Insert menus
 * @param array $menus Menu root parent titles
 */
function gemini_migrate_d5_insert_menu($menus) {

  // Get all Druapal 5 {menu} objects
  $D5_menu_items = gemini_migrate_d5_menu_array();

  // Reduce Drupal 7 {menu_custom} from array of objects to into $mc
  $mc = gemini_migrate_d5_map_menu_custom();

  // Get last auto-increment id from Drupal 7 {menu_links}
  $last_id = gemini_migrate_d5_get_menu_links_last_insert_id();

  // User selected menu(s) from the form
  foreach($menus as $m) {

    // Do insert if menu exists in Drupal 7 {menu_custom}
    if(in_array($m, arrayColumn($mc, 'title'))) {

      // Get Drupal 7 {menu_custom} associated with current menu
      $menu_custom_index = array_search($m, arrayColumn($mc, 'title'));
      $menu_custom = $mc[$menu_custom_index];

      // Create array for Drupal 7
      $D7 = array();
      // Loop through Drupal 5 menu objects
      foreach($D5_menu_items as $D5) {
        // Check if it belongs to menu_root 
        if($D5['menu_root_title'] == $menu_custom['title']) {
          // Check if menu item is menu_root
          if($D5['mid'] != $D5['menu_root']) {
            // Insert into Drupal 7 {menu_links}
            db_merge('menu_links')
              ->key(array('mlid' => $D5['mid'] + $last_id))
              ->fields(array(
                'menu_name' => $menu_custom['menu_name'],
                'mlid' => $D5['mid'] + $last_id,
                'plid' => (($D5['mid'] + $last_id) == ($D5['full_path'][1] + $last_id)) ? '0' : $D5['pid'] + $last_id,
                'link_path' => $D5['path'],
                'router_path' => (substr($D5['path'], 0,4) == 'http') ? '' : 'node/%',
                'link_title' => $D5['title'],
                'options' => 'a:0:{}',
                'module' => 'menu',
                'hidden' => ($D5['type'] & MENU_VISIBLE_IN_TREE) ? '0' : '1',
                'external' => (substr($D5['path'], 0,4) == 'http') ? '1' : '0',
                'has_children' => (gemini_migrate_d5_menu_item_has_children($D5['mid'])) ? '1' : '0',
                'expanded' => ($D5['type'] & MENU_EXPANDED) ? '1' : '0',
                'weight' => $D5['weight'],
                'depth' => count($D5['full_path'])-1,
                // 'customized' => $D5['customized'],
                'p1' => (isset($D5['full_path'][1])) ? $D5['full_path'][1] + $last_id : '0',
                'p2' => (isset($D5['full_path'][2])) ? $D5['full_path'][2] + $last_id : '0',
                'p3' => (isset($D5['full_path'][3])) ? $D5['full_path'][3] + $last_id : '0',
                'p4' => (isset($D5['full_path'][4])) ? $D5['full_path'][4] + $last_id : '0',
                'p5' => (isset($D5['full_path'][5])) ? $D5['full_path'][5] + $last_id : '0',
                'p6' => (isset($D5['full_path'][6])) ? $D5['full_path'][6] + $last_id : '0',
                'p7' => (isset($D5['full_path'][7])) ? $D5['full_path'][7] + $last_id : '0',
                'p8' => (isset($D5['full_path'][8])) ? $D5['full_path'][8] + $last_id : '0',
                'p9' => (isset($D5['full_path'][9])) ? $D5['full_path'][9] + $last_id : '0',
                'updated' => '1'
              ))
              ->execute();
          }
        }
      }


      drupal_set_message($menu_custom['menu_name']);
    }

  }

  return $D7;

}

/*
 * Create array of Drupal 5 {menu} with full path
 */
function gemini_migrate_d5_menu_array() {

  // Get all Druapal 5 {menu} objects
  $result = gemini_migrate_d5_get_menu('D5');
  $D5 = gemini_migrate_d5_menu_flat($result);

  // return gemini_migrate_d5_menu_path('800', $D5);

  // Create array for converted menu elements
  $menu = array();

  foreach($D5 as $a) {
    // Get full path of menu item
    $full_path = gemini_migrate_d5_menu_path($a['mid'], $D5);
    // Get title of menu root parent
    foreach($D5 as $menu_items) {
      if($menu_items['mid'] == $full_path[0]) {
        $menu_root_title = $menu_items['title'];
      }
    }

    $menu[] = array(
      'mid' => $a['mid'],
      'pid' => $a['pid'],
      'title' => $a['title'],
      'weight' => $a['weight'],
      'type' => $a['type'],
      'path' => $a['path'],
      'full_path' => $full_path,
      'menu_root' => $full_path[0],
      'menu_root_title' => $menu_root_title
    );
  }

  return $menu;
}