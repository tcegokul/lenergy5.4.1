// gps_parser.h
#ifndef GPS_PARSER_H
#define GPS_PARSER_H

#include <stdbool.h>
#include "driver/uart.h"

void gps_parser_init(uart_port_t uart_num);
void gps_parser_task(void *arg);
bool gps_has_fix(void);
float gps_get_latitude(void);
float gps_get_longitude(void);
const char* gps_get_local_time(void);
void gps_set_timezone_offset(int offset);
const char* gps_get_location_name(void);  // ✅ Ajout pour l'affichage du nom de la ville

float gps_get_qibla_distance(void);       // ✅ Juste la déclaration ici

#endif // GPS_PARSER_H


