/* 
    SAT_AppStorage.cpp - Library to write arduino application data from arduino->supervisor.
    Copyright (C) 2013 Jorge Ortiz for NanoSatisfi

   Copyright 2013 NanoSatisfi, Inc.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
  
*/

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <I2C_add.h>
#include "SAT_AppStorage.h"

/******************************************************************************
 * Definitions
 ******************************************************************************/


/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_AppStorage::SAT_AppStorage(uint8_t id){
    _local_address = id;
    commLayer = new OnboardCommLayer(MULTIMASTER, _local_address);
}


/******************************************************************************
 * Private functions 
 ******************************************************************************/


/******************************************************************************
 * User API
 ******************************************************************************/
/*
   Passes the experiment data to the comm layer; the comm layer enqueues if for sending
   upon request from master;
*/
void SAT_AppStorage::store_exp_data(SAT_ExperimentData* edata){
    if(edata!=NULL){
        exp_data_t* data = edata->get_exp_data();
        message_t msg;
        msg.t = APPEND;
        msg.node_addr = _local_address;
        nano_datamsg_t* datamsg = (nano_datamsg_t*)&msg.data;
        datamsg->len = sizeof(exp_data_t);
        memcpy(datamsg->buf, data, sizeof(exp_data_t));
        commLayer->send(&msg, I2C_ADD_SUPERVISOR);
    }
}

/*
   Passes the experiment data to the comm layer; the comm layer enqueues if for sending
   upon request from master;  The data comes in raw form.
*/
void SAT_AppStorage::store_exp_data(const uint8_t* buf, size_t len){
    if(buf!=NULL){
        message_t msg;
        msg.t = APPEND;
        msg.node_addr = _local_address;
        nano_datamsg_t* datamsg = (nano_datamsg_t*)&msg.data;
        datamsg->len = len;
        memcpy(datamsg->buf, buf, len);
        commLayer->send(&msg, I2C_ADD_SUPERVISOR);
    }
}
