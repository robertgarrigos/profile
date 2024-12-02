<?php
/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class ProfileMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['label'] = array(
      'label' => t('Label'),
      'description' => t('The profile label.'),
      'setter callback' => 'entity_plus_property_verbatim_set',
      'setter permission' => 'administer profiles',
      'schema field' => 'label',
    );

    $properties['type'] = array(
      'type' => 'profile_type',
      'getter callback' => 'entity_plus_property_getter_method',
      'setter callback' => 'entity_plus_property_verbatim_set',
      'setter permission' => 'administer profiles',
      'required' => TRUE,
      'description' => t('The profile type.'),
    ) + $properties['type'];

    unset($properties['uid']);

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The owner of the profile."),
      'getter callback' => 'entity_plus_property_getter_method',
      'setter callback' => 'entity_plus_property_setter_method',
      'setter permission' => 'administer profiles',
      'required' => TRUE,
      'schema field' => 'uid',
    );

    $properties['created'] = array(
      'label' => t("Date created"),
      'type' => 'date',
      'description' => t("The date the profile was created."),
      'setter callback' => 'entity_plus_property_verbatim_set',
      'setter permission' => 'administer profiles',
      'schema field' => 'created',
    );
    $properties['changed'] = array(
      'label' => t("Date changed"),
      'type' => 'date',
      'schema field' => 'changed',
      'description' => t("The date the profile was most recently updated."),
    );

    return $info;
  }
}

/**
 * Implements hook_entity_property_info_alter().
 */
function profile_entity_property_info_alter(&$info) {
  // Add related profiles to the user object.
  $properties = &$info['user']['properties'];
  foreach (profile_get_types() as $type_name => $type) {
    $properties['profile_' . $type_name] = array(
      'type' => 'profile',
      'label' => t('@type_name profile', array('@type_name' => backdrop_ucfirst($type->label))),
      'description' => t("The users's @type_name profile.", array('@type_name' => $type->label)),
      'getter callback' => 'profile_user_get_properties',
      'bundle' => $type_name,
    );
  }
}
