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
  if (!$language) {
    // Overview page will be the regular one.
    return i18n_string_translate_page_object('profile_type', $profile_type);
  }
  else {
    $language = i18n_language_object($language);
    backdrop_set_title(t('Translate to !language', array('!language' => i18n_language_name($language->langcode))));
    $profile_type_object = i18n_object('profile_type', $profile_type);
    $strings['profile_type'] = $profile_type_object->get_strings(array('empty' => TRUE));
    return backdrop_get_form('i18n_string_translate_page_form', $strings, $language->langcode);
  }
}
