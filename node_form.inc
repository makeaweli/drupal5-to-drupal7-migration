<?php

function gemini_migrate_d5_form_node() {
  // drupal_set_message('Are you sure you want to run the custom import script?  No going back without re-installing the database!!!', 'warning');

  $form['node'] = array(
    // '#type' => 'fieldset',
    // '#title' => t('Node'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE
  );

  $header = array(
    'node_type' => t('Content Type'),
    'source_total' => t('Source'),
    'destination_total' => t('Destination'),
  );

  $options = gemini_migrate_d5_get_node_type_table();

  $form['node']['table'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $options,
    '#empty' => t('No users found'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  // $result = gemini_migrate_d5_insert_field_data_body(1699);

  // $r = $result[0];
  // print_r($r->entity_id);
  return $form;
}

function gemini_migrate_d5_form_node_submit($form , $form_state) {
  $results = array_filter($form_state['values']['table']);
  drupal_set_message(print_r($results , 1));

  // Insert selected content type nodes
  $result = gemini_migrate_d5_insert_nodes($results);

}