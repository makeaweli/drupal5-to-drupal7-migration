<?php

function gemini_migrate_d5_form_user() {
  // drupal_set_message('Are you sure you want to run the custom import script?  No going back without re-installing the database!!!', 'warning');

  $form['user'] = array(
    // '#type' => 'fieldset',
    // '#title' => t('Node'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $header = array(
    'source_total' => t('Source'),
    'destination_total' => t('Destination'),
    'rollback' => t('Rollback')
  );

  $options = gemini_migrate_d5_get_form_user_table();

  $form['user']['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No users found'),
  );
  $form['user']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // $results = gemini_migrate_d5_get_users('D5');

  // drupal_set_message(print_r($results, 1));
  return $form;
}

function gemini_migrate_d5_form_user_submit($form , $form_state) {
  $results = array_filter($form_state['values']['table']);
  // drupal_set_message(print_r($results , 1));

  // Insert selected content type nodes
  gemini_migrate_d5_insert_users($results);

  echo 'test';

}