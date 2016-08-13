<?php

/**
 * @file
 * Provides Entity integration.
 */

/**
 * UI controller.
 */
abstract class ProfileTypeUIController extends DefaultEntityController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage profiles, including fields.';
    return $items;
  }
}
