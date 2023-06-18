<?php
/**
 * @file
 * Hook implementations for i18n_string module hooks.
 */

/**
 * Implements hook_i18n_string_info().
 */
function profile_i18n_i18n_string_info() {
  $groups['profile'] = array(
    'title' => 'Profile',
    'description' => 'Supports configurable user profiles.',
    'format' => FALSE,
    'list' => TRUE,
  );
  return $groups;
}

/**
 * Implements hook_i18n_object_info().
 */
function profile_i18n_i18n_object_info() {
  $info['profile_type'] = array(
    'title' => 'Profile type',
    'key' => 'type',
    // Placeholders for automatic paths.
    'placeholders' => array(
      '%profile_type' => 'type',
    ),

    // Properties for string translation.
    'string translation' => array(
      'textgroup' => 'profile',
      'type' => 'profile_type',
      'properties' => array(
        'label' => array(
          'title' => 'Label'
        ),
      ),
      'translate path' => 'admin/structure/profiles/manage/%profile_type/translate/%i18n_language',
    ),
    'edit path' => 'admin/structure/profiles/manage/%profile_type',
    'translate tab' => 'admin/structure/profiles/manage/%profile_type/translate',
  );

  return $info;
}

/** 
 * Implements hook_i18n_string_objects().
 */
function profile_i18n_i18n_string_objects() {
  return profile_get_types();
}
