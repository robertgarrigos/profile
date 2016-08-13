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
    $profile_type = entity_create('profile_type', array());
  }
  $form['profile_type'] = array(
    '#value' => $profile_type,
    '#type' => 'value',
  );

  if ($op == 'clone') {
    $profile_type->label .= ' (cloned)';
    $profile_type->type = '';
  }
//dpm($profile_type);
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
    '#disabled' => $profile_type->isLocked() && $op != 'clone',
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
    '#default_value' => '',//$profile_type->weight,
    '#description' => t('When showing profiles, those with lighter (smaller) weights get listed before profiles with heavier (larger) weights.'),
    '#weight' => 10,
  );

  if (!$profile_type->isLocked() && $op != 'add' && $op != 'clone') {
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
  entity_form_submit_build_entity('profile_type', $profile_type, $form, $form_state);
  dpm($profile_type);
  // Save and go back.
  $profile_type->save();
  $form_state['redirect'] = 'admin/structure/profiles';
}

/**
 * Form API submit callback for the delete button.
 */
function profile_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/profiles/manage/' . $form_state['profile_type']->type . '/delete';
}
