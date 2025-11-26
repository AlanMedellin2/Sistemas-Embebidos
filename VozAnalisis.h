/*
 * VozAnalisis.h
 *
 *  Created on: Nov 25, 2025
 *      Author: alang
 */


#ifndef SRC_VOZANALISIS_H_
#define SRC_VOZANALISIS_H_

#include <stdint.h>

int procesar(const int16_t *array, int length,
             float *zcrBuff,
             float *centroidBuff,
             float *bandwithBuff,
             float *rolloffBuff);

void iniciar();
void ventanaHann();
void frecuenciaBins();

float sum_f32(const float *x, int n);

float ZCR_f(const float *x, uint32_t N);
float specCentroid(const float *mag, const float *frecuencia, int nBins);
float specBandwidht(const float *mag, const float *frecuencia,
                    int nBins, float centroid);
float rolloff(const float *mag, const float *frecuencia,
              int nBins, float percent);

#endif /* SRC_VOZANALISIS_H_ */


