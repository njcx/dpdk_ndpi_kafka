//
// Created by njcx on 2025/4/2.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "kafka_config.h"

kafka_config_t* read_config(const char *config_file) {
    kafka_config_t *config = calloc(1, sizeof(kafka_config_t));
    json_error_t error;

    // 读取 JSON 配置文件
    json_t *root = json_load_file(config_file, 0, &error);
    if (!root) {
        fprintf(stderr, "Error parsing config file: %s\n", error.text);
        free(config);
        return NULL;
    }

    // 读取配置项
    json_t *brokers = json_object_get(root, "brokers");
    json_t *topic = json_object_get(root, "topic");
    json_t *security_protocol = json_object_get(root, "security_protocol");
    json_t *sasl_mechanism = json_object_get(root, "sasl_mechanism");
    json_t *sasl_username = json_object_get(root, "sasl_username");
    json_t *sasl_password = json_object_get(root, "sasl_password");

    if (json_is_string(brokers))
        config->brokers = strdup(json_string_value(brokers));
    if (json_is_string(topic))
        config->topic = strdup(json_string_value(topic));
    if (json_is_string(security_protocol))
        config->security_protocol = strdup(json_string_value(security_protocol));
    if (json_is_string(sasl_mechanism))
        config->sasl_mechanism = strdup(json_string_value(sasl_mechanism));
    if (json_is_string(sasl_username))
        config->sasl_username = strdup(json_string_value(sasl_username));
    if (json_is_string(sasl_password))
        config->sasl_password = strdup(json_string_value(sasl_password));

    json_decref(root);
    return config;
}

void free_kafka_config(kafka_config_t *config) {
    if (config) {
        free(config->brokers);
        free(config->topic);
        free(config->security_protocol);
        free(config->sasl_mechanism);
        free(config->sasl_username);
        free(config->sasl_password);
        free(config);
    }
}