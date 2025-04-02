#include <stdio.h>
#include <string.h>
#include <librdkafka/rdkafka.h>
#include <jansson.h>
#include "kafka_config.h"

// 错误回调函数
static void error_callback(rd_kafka_t *rk, int err, const char *reason, void *opaque) {
    fprintf(stderr, "%% Error: %s: %s\n", rd_kafka_err2str(err), reason);
}

// 消息传递回调函数
static void dr_msg_cb(rd_kafka_t *rk, const rd_kafka_message_t *rkmessage, void *opaque) {
    if (rkmessage->err)
        fprintf(stderr, "%% Message delivery failed: %s\n", rd_kafka_err2str(rkmessage->err));
    else
        fprintf(stderr, "%% Message delivered (%zd bytes)\n", rkmessage->len);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <config_file.json>\n", argv[0]);
        return 1;
    }

    // 读取配置文件
    kafka_config_t *config = read_config(argv[1]);
    if (!config) {
        fprintf(stderr, "Failed to read config file\n");
        return 1;
    }

    rd_kafka_t *producer;
    rd_kafka_conf_t *conf;
    rd_kafka_topic_t *topic;
    char errstr[512];

    // 创建 producer 配置对象
    conf = rd_kafka_conf_new();

    // 设置基本配置
    if (rd_kafka_conf_set(conf, "bootstrap.servers", config->brokers,
                          errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
        fprintf(stderr, "%s\n", errstr);
        return 1;
    }

    // 设置安全协议
    if (config->security_protocol) {
        if (rd_kafka_conf_set(conf, "security.protocol", config->security_protocol,
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "%s\n", errstr);
            return 1;
        }
    }

    // 设置 SASL 机制
    if (config->sasl_mechanism) {
        if (rd_kafka_conf_set(conf, "sasl.mechanism", config->sasl_mechanism,
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "%s\n", errstr);
            return 1;
        }
    }

    // 设置 SASL 用户名和密码
    if (config->sasl_username && config->sasl_password) {
        if (rd_kafka_conf_set(conf, "sasl.username", config->sasl_username,
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK ||
            rd_kafka_conf_set(conf, "sasl.password", config->sasl_password,
                              errstr, sizeof(errstr)) != RD_KAFKA_CONF_OK) {
            fprintf(stderr, "%s\n", errstr);
            return 1;
        }
    }

    // 设置消息传递回调函数
    rd_kafka_conf_set_dr_msg_cb(conf, dr_msg_cb);

    // 创建 producer 实例
    producer = rd_kafka_new(RD_KAFKA_PRODUCER, conf, errstr, sizeof(errstr));
    if (!producer) {
        fprintf(stderr, "%% Failed to create producer: %s\n", errstr);
        free_kafka_config(config);
        return 1;
    }

    // 创建 topic 配置对象
    topic = rd_kafka_topic_new(producer, config->topic, NULL);
    if (!topic) {
        fprintf(stderr, "%% Failed to create topic: %s\n",
                rd_kafka_err2str(rd_kafka_last_error()));
        rd_kafka_destroy(producer);
        free_kafka_config(config);
        return 1;
    }

    // 创建示例 JSON 数据
    json_t *json_obj = json_object();
    json_object_set_new(json_obj, "id", json_integer(1));
    json_object_set_new(json_obj, "name", json_string("test"));
    json_object_set_new(json_obj, "timestamp", json_integer(time(NULL)));

    // 将 JSON 转换为字符串
    char *json_str = json_dumps(json_obj, JSON_COMPACT);

    // 发送消息
    if (rd_kafka_produce(
            topic, RD_KAFKA_PARTITION_UA,
            RD_KAFKA_MSG_F_COPY,
            json_str, strlen(json_str),
            NULL, 0,
            NULL) == -1) {
        fprintf(stderr, "%% Failed to produce message: %s\n",
                rd_kafka_err2str(rd_kafka_last_error()));
    }

    // 等待消息发送完成
    rd_kafka_flush(producer, 10 * 1000);

    // 清理资源
    free(json_str);
    json_decref(json_obj);
    rd_kafka_topic_destroy(topic);
    rd_kafka_destroy(producer);
    free_kafka_config(config);

    return 0;
}