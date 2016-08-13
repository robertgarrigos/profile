<?php

/**
 * @file
 * Adds separate pages for viewing and editing profiles.
 */

/**
 * Shows the profile page for the current user.
 *
 * @see user_page()
 */
function profile_page_own($base_path) {
  global $user;
  if ($user->uid) {
    menu_set_active_item($base_path . '/' . $user->uid);
    return menu_execute_active_handler(NULL, FALSE);
  }
  else {
    return backdrop_get_form('user_login');
  }
}

/**
 * Profile view page.
 */
function profile_page_view($profile) {
  return $profile->view('page');
}

/**
 * The profile edit form.
 */
function profile_form($form, &$form_state, $profile) {
  if (empty($form_state['profiles'])) {
    $form_state['profiles'][$profile->type] = $profile;
  }
  // Prevent invoking the same hooks twice, so tell profile_attach_form() to
  // skip invoking the hooks.
  $form_state['profile_skip_hook'] = TRUE;
  profile_attach_form($form, $form_state);

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 40,
  );
  if (empty($profile->is_new) && user_access('administer profiles')) {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete profile'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('profile_form_submit_delete')
    );
  }
  $form['#submit'][] = 'profile_form_submit';
  return $form;
}

/**
 * Profile form submit handler.
 */
function profile_form_submit($form, &$form_state) {
  // The profile is being saved by profile_form_submit_handler().
  backdrop_set_message(t('The changes have been saved.'));
  $form_state['redirect'] = $form_state['profile']->path();
}

/**
 * Profile form submit handler for the delete button.
 */
function profile_form_submit_delete($form, &$form_state) {
  $form_state['redirect'] = $form_state['profile']->path() . '/delete';
}

/**
 * Confirm form for deleting a profile.
 */
function profile_page_delete_confirm_form($form, &$form_state, $profile) {
  $form_state += array('profile' => $profile);
  $confirm_question = t('Are you sure you want to delete profile %label?', array('%label' => $profile->label()));
  return confirm_form($form, $confirm_question, $profile->path());
}

function profile_page_delete_confirm_form_submit($form, &$form_state) {
  $profile = $form_state['profile'];
  $profile->delete();
  backdrop_set_message(t('Deleted %label.', array('%label' => $profile->label)));
  $form_state['redirect'] = 'user/' . $profile->uid;
}
