<?php

function gemini_migrate_d5_form_menu() {
  // drupal_set_message('Are you sure you want to run the custom import script?  No going back without re-installing the database!!!', 'warning');

  $form['menu'] = array(
    // '#type' => 'fieldset',
    // '#title' => t('Node'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $header = array(
    'menu_type' => t('Menu Type'),
    'source_total' => t('Source'),
    'destination_total' => t('Destination'),
    'rollback' => t('Rollback')
  );

  $options = gemini_migrate_d5_get_menu_type_table();

  $form['menu']['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No users found'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  $form['submit_import'] = array(
    '#type' => 'submit',
    '#value' => t('Start import'),
  );

  // $result = gemini_migrate_d5_get_menu('D5');
  // $result = gemini_migrate_d5_menu_flat($result);
  // // print_r($result);
  // $result = gemini_migrate_d5_menu_path('788', $result);

  // for($i=0; $i<312; $i++) {
  //  drupal_set_message(print_r(gemini_migrate_d5_menu_path($result[$i]['mid'], $result)));
  // }

  // drupal_set_message($result[311]['mid']);
  // drupal_set_message(print_r(gemini_migrate_d5_menu_path($result[311]['mid'], $result)));
  // drupal_set_message(print_r(gemini_migrate_d5_menu_path('5', $result)));

  // $result = gemini_migrate_d5_menu_array();

  // print_r($result);

  // drupal_set_message(print_r(gemini_migrate_d5_menu_path('921', $result)));

  // $m7 = gemini_migrate_d5_get_menu_types('D7');

  // print_r($m7);

  // print_r(gemini_migrate_d5_map_menu_custom());
  // $menu_customs = gemini_migrate_d5_map_menu_custom();

  // $menu_custom_index = array_search('Science Operations', arrayColumn($menu_customs, 'title'));
  // $menu_custom = $menu_customs[$menu_custom_index];
  

  // print_r($menu_custom);

 // echo (substr('http://www.gemini.edu', 0,4) == 'http') ? 'matches' : 'no match';

  return $form;
}

function gemini_migrate_d5_form_menu_submit($form , $form_state) {
  $results = array_filter($form_state['values']['table']);
  // drupal_set_message(print_r($results , 1));

  // Insert selected content type nodes
  $result = gemini_migrate_d5_insert_menu($results);

  drupal_set_message(print_r($result , 1));

}