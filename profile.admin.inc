<?php

/**
 * @file
 * Profile type editing UI.
 */

/**
 * Generates the profile type editing form.
 */
function profile_type_form($form, &$form_state, $profile_type, $op = 'edit') {
  if (!isset($profile_type) && $op == 'add') {
    $profile_type = profile_type_set_defaults();
  }
  $form['#profile_type'] = $profile_type;
  $form['profile_type'] = array(
    '#value' => $profile_type,
    '#type' => 'value',
  );

  if ($op == 'clone') {
    $profile_type->label .= ' (cloned)';
    $profile_type->type = '';
  }

  $form['label'] = array(
    '#title' => t('Label'),
    '#type' => 'textfield',
    '#default_value' => $profile_type->label,
    '#description' => t('The human-readable name of this profile type.'),
    '#required' => TRUE,
    '#size' => 30,
  );
  // Machine-readable type name.
  $form['type'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($profile_type->type) ? $profile_type->type : '',
    '#maxlength' => 32,
    '#disabled' => ($profile_type->storage == PROFILE_STORAGE_DEFAULT) && $op != 'clone',
    '#machine_name' => array(
      'exists' => 'profile_get_types',
      'source' => array('label'),
    ),
    '#description' => t('A unique machine-readable name for this profile type. It must only contain lowercase letters, numbers, and underscores.'),
  );

  $form['data']['#tree'] = TRUE;
  $form['data']['registration'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show during user account registration.'),
    '#default_value' => !empty($profile_type->data['registration']),
  );

  $user_roles = user_roles();
  // Exclude anonymous user role.
  unset($user_roles[BACKDROP_ANONYMOUS_ROLE]);
  $form['data']['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Roles'),
    '#description' => t('Check user roles that should have this profile.'),
    '#options' => $user_roles,
    '#default_value' => !empty($profile_type->data['roles']) ? $profile_type->data['roles'] : array_keys($user_roles)
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save profile type'),
    '#weight' => 40,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => $profile_type->weight,
    '#description' => t('When showing profiles, those with lighter (smaller) weights get listed before profiles with heavier (larger) weights.'),
    '#weight' => 10,
  );

  if ($profile_type->storage === PROFILE_STORAGE_NORMAL && $op != 'add' && $op != 'clone') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('profile_type_form_submit_delete')
    );
  }
  
  return $form;
}

/**
 * Form API submit callback for the type form.
 */
function profile_type_form_submit(&$form, &$form_state) {
  $form_state['values']['data']['roles'] = array_filter($form_state['values']['data']['roles']);
  $profile_type = $form_state['values']['profile_type'];
  $profile_type->label = $form_state['values']['label'];
  $profile_type->type = $form_state['values']['type'];
  $profile_type->data = $form_state['values']['data'];

  profile_type_save($profile_type);
  $form_state['redirect'] = 'admin/structure/profiles';
}

/**
 * Form API submit callback for the delete button.
 */
function profile_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/profiles/manage/' . $form['#profile_type']->type . '/delete';
}

/**
 * Form API submit callback for the delete button.
 */
function profile_type_delete_form($form, &$form_state, $profile_type) {
  $form_state['profile_type'] = $profile_type;
  $confirm_question = t('Are you sure you want to delete the %label profile type?', array('%label' => $profile_type->label));
  return confirm_form($form, $confirm_question, 'admin/structure/profiles');
}

/**
 * Form API submit callback for the delete button.
 */
function profile_type_delete_form_submit($form, &$form_state) {
  profile_type_delete($form_state['profile_type']);

  backdrop_set_message(t('The profile type %name has been deleted.', array('%name' => $form_state['profile_type']->type)));

  $form_state['redirect'] = 'admin/structure/profiles/manage';
  return;
}

/**
 * Builds the entity overview form.
 */
function profile_type_overview_form($form, &$form_state) {
  $entities = profile_get_types();

  $rows = array();
  foreach ($entities as $entity) {
    $rows[] = profile_type_overview_form_row($entity->type, $entity);
  }

  $render = array(
    '#theme' => 'table',
    '#header' => array(t('Label'), t('Status'), array('data' => t('Operations'), 'colspan' => 4)),
    '#rows' => $rows,
    '#empty' => t('None.'),
  );

  $form['table'] = $render;
  $form['pager'] = array('#theme' => 'pager');
  return $form;
}

/**
 * Generates the row for the passed entity and may be overridden in order to
 * customize the rows.
 */
function profile_type_overview_form_row($id, $entity) {
  $entity_uri = 'admin/structure/profiles';

  $row[] = $entity->label;

  // Add a row for the exportable status.
  if ($entity->storage === PROFILE_STORAGE_OVERRIDE) {
    $storage = t('Overridden');
  }
  elseif ($entity->storage === PROFILE_STORAGE_DEFAULT) {
    $storage = t('Default (module-provided)');
  }
  else {
    $storage = t('Custom');
  }
  $row[] = $storage;

  $operations = array(
    '#type' => 'dropbutton',
    '#links' => _profile_get_operations($id, $entity),
  );
  $row[] = backdrop_render($operations);

  return $row;
}


