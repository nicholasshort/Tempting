/*
 * zcl.co2.meas.h
 *
 *  Created on: Jun 19, 2025
 *      Author: nick
 */

#ifndef INC_ZCL_CO2_MEAS_H_
#define INC_ZCL_CO2_MEAS_H_

#include "zcl/zcl.h"

/* Cluster ID for Carbon Dioxide Measurement */
#define ZCL_CLUSTER_CO2_MEASUREMENT      0x040d

/** CO2 Measurement Server Attribute IDs */
enum ZbZclCo2MeasSvrAttrT {
    ZCL_CO2_MEAS_ATTR_MEAS_VAL = 0x0000,       /**< MeasuredValue (ppm) */
    ZCL_CO2_MEAS_ATTR_MIN_MEAS_VAL = 0x0001,   /**< MinMeasuredValue */
    ZCL_CO2_MEAS_ATTR_MAX_MEAS_VAL = 0x0002,   /**< MaxMeasuredValue */
    ZCL_CO2_MEAS_ATTR_TOLERANCE = 0x0003       /**< Tolerance (optional) */
};

/* Allocate server cluster */
struct ZbZclClusterT * ZbZclCo2MeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    float min_measured_value, float max_measured_value, float tolerance);


#endif /* INC_ZCL_CO2_MEAS_H_ */
