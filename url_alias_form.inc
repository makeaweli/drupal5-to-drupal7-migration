<?php

// Copy Drupal 5 constant
define('MENU_EXPANDED', 0x0100);

function gemini_migrate_d5_form_url_alias() {
  // drupal_set_message('Are you sure you want to run the custom import script?  No going back without re-installing the database!!!', 'warning');

  $form['menu'] = array(
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $header = array(
    'alias_type' => t('Alias Type'),
    'source_total' => t('Source'),
    'destination_total' => t('Destination'),
  );

  $options = gemini_migrate_d5_get_url_alias_type_table();

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

  return $form;
}

function gemini_migrate_d5_form_url_alias_submit($form , $form_state) {
  $results = array_filter($form_state['values']['table']);
  // drupal_set_message(print_r($results , 1));

  // Insert selected content type nodes
  $result = gemini_migrate_d5_insert_url_alias($results);

  // drupal_set_message(print_r($result , 1));

}