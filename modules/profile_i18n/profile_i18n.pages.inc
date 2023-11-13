<?php
/**
 * @file
 * Page callbacks for Profile i18n integration module.
 */

/**
 * Translate page callback.
 * 
 * @param stdClass $profile_type
 *  The profile type object.
 * @param stdClass $language
 *  The language object.
 * 
 * @return array
 *  The form array.
 */
function profile_i18n_page_translate($profile_type, $language = NULL) {
  module_load_include('inc', 'i18n_string', 'i18n_string.pages');
  return i18n_string_translate_page_object('profile_type', $profile_type, $language);
}
