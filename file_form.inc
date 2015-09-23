<?php

function gemini_migrate_d5_form_file() {
  // drupal_set_message('Are you sure you want to run the custom import script?  No going back without re-installing the database!!!', 'warning');

  $form['file'] = array(
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $header = array(
    'content_type' => t('Content Type'),
    'source_total' => t('Source'),
    'destination_total' => t('Destination'),
  );

  $options = gemini_migrate_d5_get_file_type_table();

  $form['url_alias']['table'] = array(
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

  // $file_revisions = gemini_migrate_d5_get_file_revisions();
  // $result = array_filter($file_revisions, array(new GeminiMigrateD5UtilityFile('287', 'fid'), 'isEqual'));
  // $vids = arrayColumn($result, 'vid');
  // //  rsort($vids);
  // $vid = $vids[0];

  // $result = array_filter(gemini_migrate_d5_get_node_revisions(), array(new GeminiMigrateD5UtilityFile($vid, 'vid'), 'isEqual'));

  // $result = array_values($result);
  // print_r($result[0]['uid']);
  // 
  // $result = gemini_migrate_d5_get_current_file_revisions('blog');

  // print_r($result);

  return $form;
}

function gemini_migrate_d5_form_file_submit($form , $form_state) {
  $results = array_filter($form_state['values']['table']);
  // drupal_set_message(print_r($results , 1));

  // Insert selected content type nodes
  $result = gemini_migrate_d5_insert_file($results);

  drupal_set_message(print_r($result , 1));

}