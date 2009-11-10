<?php
// $Id$
/**
 * @file
 * Contains Image module integration for the File Aliases module.
 */

/**
 * Implements file_aliases_hook_process().
 */
function file_aliases_image_process(&$node, &$file, &$path, &$alias, &$count) {
  $count[$file['name']] = isset($count[$file['name']]) ? $count[$file['name']] + 1 : 1;
  $target = &$node->content[$file['name']]['items'][$count[$file['name']] - 1]['#item'];
  $target['origpath'] = $file['field']['uri'];
  $target['uri'] = $alias;
  $target['filename'] = array_pop(explode('/', $alias));
}

/**
 * Implements file_aliases_hook_views_handlers().
 */
//function file_aliases_image_views_handlers(&$handlers) {
//  $handlers['file_aliases_content_handler_field_multiple'] = array(
//    'parent' => 'file_aliases_content_handler_field',
//  );
//  $handlers['file_aliases_content_handler_field'] = array(
//    'parent' => 'content_handler_field',
//  );
//}

/**
 * Implements file_aliases_hook_views_data_alter().
 */
//function file_aliases_image_views_data_alter(&$data) {
//  foreach (content_fields() as $field) {
//    if ($field['module'] == 'image' && isset($data['node_data_' . $field['field_name']])) {
//      $data['node_data_' . $field['field_name']][$field['field_name'] . '_fid']['field']['handler'] = 'file_aliases_content_handler_field_multiple';
//    }
//  }
//}

/**
 * Implements hook_theme_registry_alter().
 */
function file_aliases_theme_registry_alter(&$theme_registry) {
  $theme_registry['field_formatter_image']['theme path'] = drupal_get_path('module', 'file_aliases');
  $theme_registry['field_formatter_image']['function'] = 'theme_file_aliases_field_formatter_image';

  foreach (image_styles() as $style) {
    $theme_registry['field_formatter_image__' . $style['name']]['theme path'] = drupal_get_path('module', 'file_aliases');
    $theme_registry['field_formatter_image__' . $style['name']]['function'] = 'theme_file_aliases_field_formatter_image';
  }
}

/**
 * Modified version of theme_field_formatter_image().
 *
 * Theme function for 'image' image field formatter.
 */
function theme_file_aliases_field_formatter_image($variables) {
  $element = $variables['element'];
  $image = array(
    'path' => $element['#item']['uri'],
    'alt' => $element['#item']['alt'],
    'title' => $element['#item']['title'],
  );

  // Check if this requires a particular image style.
  $matches = array();
  if (preg_match('/__([a-z0-9_]+)/', $element['#formatter'], $matches)) {
    $image['style_name'] = $matches[1];
    $image['path'] = $element['#item']['origpath'];
    return theme('image_style', $image);
  }
  else {
    $attributes = @getimagesize($element['#item']['origpath']);
    $image += array(
      'attributes' => array(
        'width' => $attributes[0],
        'height' => $attributes[1],
      ),
      'getsize' => FALSE,
    );
    return theme('image', $image);
  }
}

/**
 * Implements CCK's hook_field($op = 'sanitize').
 */
//function file_aliases_image_field($op, $node, $field, &$items, $teaser, $page) {
//  if ($op == 'sanitize') {
//    foreach ($items as $delta => $item) {
//      // Cleanup $items during node preview.
//      if (empty($item['fid']) || !empty($item['delete'])) {
//        // Check for default images at the widget level.
//        // TODO: Provide an API to ImageField to do this itself?
//        if (!empty($field['widget']['use_default_image']) && !empty($field['widget']['default_image']['filepath'])) {
//          $items[$delta] = $field['widget']['default_image'];
//          $items[$delta]['default'] = TRUE;
//        }
//        else {
//          $items[$delta] = NULL;
//          continue;
//        }
//      }
//      // Load the complete file if a filepath is not available.
//      if (!empty($item['fid']) && empty($item['filepath'])) {
//        $items[$delta] = array_merge($item, field_file_load($item['fid']));
//      }
//      // Add nid so formatters can create a link to the node.
//      $items[$delta]['nid'] = $node->nid;
//
//      // Calculate relative path.
//      $path = (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PUBLIC) ? '' : base_path();
//      foreach (explode('/', file_directory_path()) as $dir) {
//        $path .= '../';
//      }
//
//      if (($alias = drupal_get_path_alias('image_paths/alias/' . $items[$delta]['fid'])) !== 'image_paths/alias/' . $items[$delta]['fid'] && _file_aliases_display_alias($field['type_name'], $field['field_name'])) {
//        $items[$delta]['origpath'] = $items[$delta]['filepath'];
//        $items[$delta]['filepath'] = file_directory_path() . '/' . $path . $alias;
//      }
//
//      // TODO: This is only necessary for Views, which doesn't call the "load"
//      // $op. It might be preferable to move this to Views integration somehow.
//      if (!empty($items['data']) && is_string($items[$delta]['data'])) {
//        $item['data'] = unserialize($item['data']);
//      }
//      // Temporary fix to unserialize data serialized multiple times.
//      // See the FileField issue http://drupal.org/node/402860.
//      // And the CCK issue http://drupal.org/node/407446.
//      while (!empty($items[$delta]['data']) && is_string($items[$delta]['data'])) {
//        $items[$delta]['data'] = unserialize($items[$delta]['data']);
//      }
//
//      // Verify the file exists on the server.
//      if (!empty($item['filepath']) && !file_exists($item['filepath'])) {
//        watchdog('image', 'FileField was trying to display the file %file, but it does not exist.', array('%file' => $item['filepath']));
//      }
//    }
//  }
//}
