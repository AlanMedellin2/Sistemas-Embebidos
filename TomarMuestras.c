/*
 * VozAnalisis.c
 *  Created on: Nov 25, 2025
 *      Author: alang
 */

#include "VozAnalisis.h"

#include "stdbool.h"
#include "stm32h7xx.h"
#include "stdio.h"
#include "arm_math.h"
#include "math.h"
#include <string.h>
#include <stdint.h>

#define ADC_VREF 3.3f
#define FS      16000.0f
#define FRAME   512
#define HOP     (FRAME/2)
#define LEN_SIGNAL 32000
#define MAX_FRAMES ((LEN_SIGNAL - FRAME) / HOP + 2)
#define PI 3.14159f

/* Estado FFT */
static arm_rfft_fast_instance_f32 fft_inst;

/* Buffers */
static float hann[FRAME];
static float frecuenciaBin[FRAME/2];

static float spectrum[FRAME];
static float frame_buff[FRAME];
static float mag[FRAME/2];

//INICIO

void iniciar() {

    arm_rfft_fast_init_f32(&fft_inst, FRAME);

    ventanaHann();
    frecuenciaBins();
}

void ventanaHann() {
    for (int i = 0; i < FRAME; i++) {
        hann[i] = 0.5f - 0.5f * cosf((2.0f * PI * i) / (FRAME - 1));
    }
}

void frecuenciaBins() {
    for (int i = 0; i < FRAME/2; i++) {
        frecuenciaBin[i] = (float)i * FS / (float)FRAME;
    }
}

//SUMATORIA
float sum_f32(const float *x, int n) {
    float s = 0.0f;
    for (int i = 0; i < n; i++) {
        s += x[i];
    }
    return s;
}

//CARACTERISTICAS

float ZCR_f(const float *x, uint32_t N) {
    int cont = 0;
    for (uint32_t i = 1; i < N; i++) {
        if ((x[i] > 0.0f && x[i - 1] < 0.0f) ||
            (x[i] < 0.0f && x[i - 1] > 0.0f)) {
            cont++;
        }
    }
    return (float)cont / (float)(N - 1);
}

float specCentroid(const float *mag, const float *frecuencia, int nBins) {

    float num = 0.0f, den = 0.0f;

    arm_dot_prod_f32(frecuencia, mag, nBins, &num);
    den = sum_f32(mag, nBins);

    if (den < 1e-12f) return 0.0f;

    return num / den;
}

float specBandwidht(const float *mag, const float *frecuencia,
                    int nBins, float centroid) {

    float num = 0.0f, den = 0.0f;

    for (int i = 0; i < nBins; i++) {
        float diff = frecuencia[i] - centroid;
        num += mag[i] * diff * diff;
        den += mag[i];
    }

    if (den < 1e-12f) return 0.0f;

    float var = num / den;
    if (var < 0.0f) var = 0.0f;

    return sqrtf(var);
}

float rolloff(const float *mag, const float *frecuencia,
              int nBins, float percent) {

    float total = sum_f32(mag, nBins);
    if (total < 1e-12f) return 0.0f;

    float threshold = percent * total;
    float acc = 0.0f;

    for (int i = 0; i < nBins; i++) {
        acc += mag[i];
        if (acc >= threshold) {
            return frecuencia[i];
        }
    }

    return frecuencia[nBins - 1];
}

//PROCESAR LA SEÑAL

int procesar(const int16_t *array, int length,
             float *zcrBuff,
             float *centroidBuff,
             float *bandwithBuff,
             float *rolloffBuff) {

    iniciar();

    int frameCont = 0;

    for (int i = 0; i + FRAME <= length; i += HOP) {

        /* ---- 1. REMOVER DC ---- */
        float mean = 0.0f;
        for (int j = 0; j < FRAME; j++) {
            mean += (float)array[i + j];
        }
        mean /= FRAME;

        /* ---- 2. NORMALIZAR ---- */
        for (int j = 0; j < FRAME; j++) {
            float centered = (float)array[i + j] - mean;
            frame_buff[j] = centered / 32768.0f;
        }

        /* ---- 3. VENTANA ---- */
        arm_mult_f32(frame_buff, hann, frame_buff, FRAME);

        /* ---- 4. FFT ---- */
        arm_rfft_fast_f32(&fft_inst, frame_buff, spectrum, 0);

        /* ---- 5. MAGNITUD ---- */
        arm_cmplx_mag_f32(spectrum, mag, FRAME/2);

        /* ---- 6. FEATURES ---- */
        float zcr = ZCR_f(frame_buff, FRAME);
        float centroid = specCentroid(mag, frecuenciaBin, FRAME/2);
        float bw = specBandwidht(mag, frecuenciaBin, FRAME/2, centroid);
        float roll = rolloff(mag, frecuenciaBin, FRAME/2, 0.85f);

        /* ---- 7. GUARDAR ---- */
        if (frameCont < MAX_FRAMES) {
            zcrBuff[frameCont] = zcr;
            centroidBuff[frameCont] = centroid;
            bandwithBuff[frameCont] = bw;
            rolloffBuff[frameCont] = roll;
        }

        frameCont++;
        if (frameCont >= MAX_FRAMES) break;
    }

    return frameCont;
}
