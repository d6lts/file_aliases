<?php
/**
 * @file
 * Contains File module integration for the File Aliases module.
 */

/**
 * Implements file_aliases_hook_process().
 */
function file_aliases_file_process(&$node, &$file, &$path, &$alias, &$count) {
  $count[$file['name']] = isset($count[$file['name']]) ? $count[$file['name']] + 1 : 1;
  $target = &$node->content[$file['name']]['items'][$count[$file['name']] - 1]['#item'];
  $target['origpath'] = $file['field']['uri'];
  $target['uri'] = $alias;
  $target['filename'] = array_pop(explode('/', $alias));
}

/**
 * Implements file_aliases_hook_views_handlers().
 */
function file_aliases_file_views_handlers(&$handlers) {
  $handlers['file_aliases_content_handler_field_multiple'] = array(
    'parent' => 'file_aliases_content_handler_field',
  );
  $handlers['file_aliases_content_handler_field'] = array(
    'parent' => 'content_handler_field',
  );
}

/**
 * Implements file_aliases_hook_views_data_alter().
 */
function file_aliases_file_views_data_alter(&$data) {
  foreach (content_fields() as $field) {
    if ($field['module'] == 'file' && isset($data['node_data_' . $field['field_name']])) {
      $data['node_data_' . $field['field_name']][$field['field_name'] . '_fid']['field']['handler'] = 'file_aliases_content_handler_field_multiple';
    }
  }
}

/**
 * Implements CCK's hook_field($op = 'sanitize').
 */
//function file_aliases_file_field($op, $node, $field, &$items, $teaser, $page) {
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
//      if (($alias = drupal_get_path_alias('file_paths/alias/' . $items[$delta]['fid'])) !== 'file_paths/alias/' . $items[$delta]['fid'] && _file_aliases_display_alias($field['type_name'], $field['field_name'])) {
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
//        watchdog('file', 'FileField was trying to display the file %file, but it does not exist.', array('%file' => $item['filepath']));
//      }
//    }
//  }
//}
