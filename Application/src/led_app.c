/**
 * File description
*/
#include <stdio.h>

#include "mjson.h"

#define SIZE 50
#define LED_MANAGER_ALL_NUM 2

typedef struct led_attrs led_attrs_t;
struct led_attrs
{
    int ledIndex;
    int ledStatus;
    int colorIndex;
    int lightDuty;
};

typedef struct config config_t;
struct config
{
    char action[SIZE];
    int nObject;
    struct led_attrs ledList[LED_MANAGER_ALL_NUM];
};

/**
 * Parsing the JSON object into the C structure
*/
int json_config_read(const char *buf, struct config *config)
{
    /* Mapping of JSON attributes to C led_attrs's struct members */
    const struct json_attr_t json_attrs_led[] = {
        {"ledIndex", t_integer, STRUCTOBJECT(struct led_attrs, ledIndex)},
        {"ledStatus", t_integer, STRUCTOBJECT(struct led_attrs, ledStatus)},
        {"colorIndex", t_integer, STRUCTOBJECT(struct led_attrs, colorIndex)},
        {"lightDuty", t_integer, STRUCTOBJECT(struct led_attrs, lightDuty)},
        {NULL},
    };

    /* Mapping of JSON attributes to C led_attrs's struct members */
    const struct json_attr_t json_attrs_config[] = {
        {"action", t_string, STRUCTOBJECT(struct config, action), 
                    .addr.string = config->action ,.len = sizeof(config->action)},
        {"ledList", t_array, STRUCTARRAY(config->ledList, json_attrs_led, &(config->nObject))},
        {NULL},
    };
    
    memset(config, '\0', sizeof(*config));

    /* Parse the JSON string from buffer */
    return json_read_object(buf, json_attrs_config, NULL);
}

int main(int argc, char const *argv[])
{
    if (argc != 2) {
        printf("Usage: ./led_app.exe <input JSON string>\n");
        return -1;
    }
    
    FILE *configFile;
    char *data;
    int fileSize = 0;
    size_t size;

    configFile = fopen(argv[1], "r");
    fseek(configFile, 0L, SEEK_END);
    fileSize = ftell(configFile);
    fseek(configFile, 0L, SEEK_SET);
    data = (char*)malloc(fileSize + 1);
    size = fread(data, 1, fileSize, configFile);
    data[size] = 0;
    fclose(configFile);

    config_t *config_info = malloc(sizeof(config_t));
    int status = json_config_read(data, config_info);
    if (status == 0)
    {
        printf("action: %s\n\n", config_info->action);
        for (size_t i = 0; i < config_info->nObject; i++)
        {
            printf("ledIndex: %d\n", config_info->ledList[i].ledIndex);
            printf("ledStatus: %s\n", ((config_info->ledList[i].ledStatus == 1) ? "ON": "OFF"));
            printf("colorIndex: %d\n", config_info->ledList[i].colorIndex);
            printf("lightDuty: %d\n", config_info->ledList[i].colorIndex);
        }
    }
    else
    {
        puts(json_error_string(status));
    }
    return 0;
}
