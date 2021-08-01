<?php
/**
 * @file
 * Theme functions for the Profile module.
 */

/**
 * Process variables for profile.tpl.php.
 */
function template_preprocess_profile(&$variables) {
  $variables['view_mode'] = $variables['elements']['#view_mode'];
  $profile = $variables['elements']['#profile'];
  $variables[$variables['elements']['#entity_type']] = $profile;

  $variables['title'] = check_plain($profile->label);

  $uri = entity_uri('profile', $profile);
  $variables['url'] = $uri && !empty($uri['path']) ? url($uri['path'], $uri['options']) : FALSE;
  $variables['page'] = $uri && !empty($uri['path']) && $uri['path'] == $_GET['q'];

  // Helpful $content variable for templates.
  $variables['content'] = array();
  foreach (element_children($variables['elements']) as $key) {
    $variables['content'][$key] = $variables['elements'][$key];
  }

  field_attach_preprocess('profile', $profile, $variables['content'], $variables);

  // Gather css classes.
  $variables['classes_array'][] = backdrop_html_class('profile-' . 'profile');

  // Add RDF type and about URI.
  if (module_exists('rdf')) {
    $variables['attributes_array']['about'] = empty($uri['path']) ? NULL: url($uri['path']);
    $variables['attributes_array']['typeof'] = empty($profile->rdf_mapping['rdftype']) ? NULL : $profile->rdf_mapping['rdftype'];
  }

  // Add suggestions.
  $variables['theme_hook_suggestions'][] = 'profile';
  $variables['theme_hook_suggestions'][] = 'profile' . '__' . $variables['view_mode'];
  $variables['theme_hook_suggestions'][] = 'profile' . '__' . $profile->type;
}
