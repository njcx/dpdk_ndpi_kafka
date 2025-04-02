#ifndef KAFKA_CONFIG_H
#define KAFKA_CONFIG_H

typedef struct {
    char *brokers;
    char *topic;
    char *security_protocol;
    char *sasl_mechanism;
    char *sasl_username;
    char *sasl_password;
} kafka_config_t;

kafka_config_t* read_config(const char *config_file);
void free_kafka_config(kafka_config_t *config);

#endif