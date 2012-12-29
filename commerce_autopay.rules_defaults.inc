<?php
/*
 * @file commerce_autopay.rules_defaults.inc
 * Provides default rules configurations for commerce recurring
 * @copyright Copyright(c) 2011 Lee Rowlands
 * @license GPL v2 http://www.fsf.org/licensing/licenses/gpl.html
 * @author Lee Rowlands contact at rowlandsgroup dot com
 * 
 */
/**
 * Implementation of hook_default_rules_configuration().
 */
function commerce_autopay_default_rules_configuration() {
  $items = array();
  $items['commerce_autopay_process_orders'] = entity_import('rules_config', '{ "commerce_autopay_process_orders" : {
      "LABEL" : "Generate Recurring Orders",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Commerce Recurring" ],
      "REQUIRES" : [ "commerce_autopay", "rules" ],
      "ON" : [ "cron" ],
      "DO" : [
        { "commerce_autopay_load_recurring" : { "PROVIDE" : { "orders" : { "orders" : "Recurring Orders" } } } },
        { "LOOP" : {
            "USING" : { "list" : [ "orders" ] },
            "ITEM" : { "order" : "Order" },
            "DO" : [
              { "component_commerce_autopay_generate_orders" : { "order" : [ "order" ] } }
            ]
          }
        }
      ]
    }
  }');
  $items['commerce_autopay_generate_orders'] = entity_import('rules_config', '{ "commerce_autopay_generate_orders" : {
      "LABEL" : "Generate Recurring Order if Appropriate",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "commerce_autopay" ],
      "USES VARIABLES" : { "order" : { "label" : "Order", "type" : "commerce_order" } },
      "IF" : [
        { "commerce_autopay_rules_order_contains_recurring" : { "commerce_order" : [ "order" ] } },
        { "commerce_autopay_rules_order_is_master" : { "commerce_order" : [ "order" ] } }
      ],
      "DO" : [
        { "commerce_autopay_process_recurring" : {
            "USING" : { "commerce_order" : [ "order" ] },
            "PROVIDE" : { "recurring_order" : { "recurring_order" : "New Order" } }
          }
        }
      ]
    }
  }');
  return $items;
}
