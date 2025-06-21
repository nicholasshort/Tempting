/*
 * z.co2.meas.c
 *
 *  Created on: Jun 19, 2025
 *      Author: nick
 */


#include "zcl.co2.meas.h"

struct ZbZclClusterT * ZbZclCo2MeasServerAlloc(struct ZigBeeT *zb, uint8_t endpoint,
    float min_measured_value, float max_measured_value, float tolerance)
{
    struct ZbZclClusterT *cluster;

    static struct ZbZclAttrT attr_list[] = {
        {
            ZCL_CO2_MEAS_ATTR_MEAS_VAL,
			ZCL_DATATYPE_FLOATING_SINGLE,
			ZCL_ATTR_FLAG_NONE,
            0,
            NULL,
            {0.0f, 1.0f},
			{0, 0}
        },
        {
            ZCL_CO2_MEAS_ATTR_MIN_MEAS_VAL,
			ZCL_DATATYPE_FLOATING_SINGLE,
			ZCL_ATTR_FLAG_NONE,
            0,
            NULL,
            {0.0f, 1.0f},
			{0, 0}
        },
        {
            ZCL_CO2_MEAS_ATTR_MAX_MEAS_VAL,
			ZCL_DATATYPE_FLOATING_SINGLE,
			ZCL_ATTR_FLAG_NONE,
            0,
            NULL,
            {0.0f, 1.0f},
			{0, 0}
        },
        {
            ZCL_CO2_MEAS_ATTR_TOLERANCE,
			ZCL_DATATYPE_FLOATING_SINGLE,
			ZCL_ATTR_FLAG_NONE,
            0,
            NULL,
            {0, 0},
			{0, 0}
        }
    };


    /* Correct alloc size: base cluster + any cluster-specific state (if needed) */
    cluster = ZbZclClusterAlloc(zb,
        sizeof(struct ZbZclClusterT), // <-- Allocate base struct size
        ZCL_CLUSTER_CO2_MEASUREMENT,
        endpoint,
        ZCL_DIRECTION_TO_SERVER);

    if (cluster == NULL) {
        return NULL;
    }

    enum ZclStatusCodeT status;

    status = ZbZclAttrAppendList(cluster, attr_list, ZCL_ATTR_LIST_LEN(attr_list));
    if (status != ZCL_STATUS_SUCCESS) {
        ZbZclClusterFree(cluster);
        return NULL;
    }
//	  NEED TO WRITE FLOAT NOT INTEGER (PERHAPS THIS MIGHT NOT EVEN BE NECESSARY)
//    status = ZbZclAttrIntegerWrite(cluster, ZCL_CO2_MEAS_ATTR_MIN_MEAS_VAL, min_measured_value);
//    if (status != ZCL_STATUS_SUCCESS) {
//    	ZbZclClusterFree(cluster);
//		return NULL;
//    }
//
//    status = ZbZclAttrIntegerWrite(cluster, ZCL_CO2_MEAS_ATTR_MAX_MEAS_VAL, max_measured_value);
//    if (status != ZCL_STATUS_SUCCESS) {
//		ZbZclClusterFree(cluster);
//		return NULL;
//	}

    return cluster;
}
