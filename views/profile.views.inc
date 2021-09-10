<?php
/**
 * @file
 * Provide views data and handlers for profile.module.
 *
 * @ingroup views_module_handlers
 */

/**
 * Implements hook_views_data().
 */
function profile_views_data() {
  $data['profile']['table']['group'] = t('Profile');
  
  $data['profile']['table']['base'] = array(
    'field' => 'pid',
    'title' => t('Profile'),
    'weight' => -10,
  );

  $data['profile']['table']['join'] = array(
    'user' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  $data['profile']['uid'] = array(
    'title' => t('User id'),
    'relationship' => array(
      'base' => 'users',
      'base field' => 'uid',
      'handler' => 'views_handler_relationship',
      'label' => t('User'),
      'title' => t('Referenced user'),
      'help' => t('User referenced by this profile'),
    ),
  );

  $data['profile']['pid'] = array(
    'title' => t('Pid'),
    'help' => t('The profile ID.'), 
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_numeric',
      'real field' => 'pid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['profile']['label'] = array(
    'title' => t('Label'),
    'help' => t('The label of the profile'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler'=> 'views_handler_argument_string',
    ),
  );

  $data['profile']['created'] = array(
    'title' => t('Date created'),
    'help' => t('The date the profile was created'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'is date' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'is date' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'is date' => TRUE,
    ),
  );

  $data['profile']['changed'] = array(
    'title' => t('Date changed'),
    'help' => t('The date the profile was changed'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
      'is date' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
      'is date' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
      'is date' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_date',
      'is date' => TRUE,
    ),
  );

  return $data;
}

/**
 * Implements hook_views_data_alter().
 */
function profile_views_data_alter(&$data) {
  // Create a relationship field to the profile entity in the users Views. This allows for views to add a relationship to the profile referencing the user  
  $data['users']['profile'] = array(
    'title' => 'Profile',
    'help' => t('Associated profile via profile\'s uid'),
    'relationship' => array(
      'label' => t('Profile'),
      'handler' => 'views_handler_relationship',
      'base' => 'profile',
      'base field' => 'uid',
      'field' => 'uid',
      'title' => t('Profile linked to the user'),
    ),
  );
}
