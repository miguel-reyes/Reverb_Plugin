/**
 * @file mf_allpass.h
 * @author Marquis Fields & Miguel Reyes Botello <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * A simple allpassfilter <br>
 * <br>
 * @brief Audio Object for generating delays using allpassfilter structure <br>
 * <br>
 * mf_allpass allows for generating multiple delays of desired length<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#ifndef mf_allpass_h
#define mf_allpass_h
#include <stdio.h>
#include <stdlib.h>

/**
 * @struct mf_allpass
 * @brief A structure for a allpass filter <br>
 * @var mf::delay The parameter value for adjusting the <br>
 * delay in Samples of the single tap delay
 * @var mf::counter A value for indexing the current sample <br>
 * @var mf::gain The parameter value for the recursive gain <br>
 * @var mf::buffer An array to store the delayed samples <br>
 */


typedef struct mf_allpass
{
    int delay;  /**< parameter for adjusting the delay of the multi tap delay */
    int counter;    /**< parameter for indexing the current sample */
    float gain;     /**< parameter for adjusting the level of the recursive gain */
    float buffer[10000]; /**< An array to store the delayed samples */
    
} mf_allpass;

/**
 * @related mf_allpass
 * @brief Creates a new allpassfilter object<br>
 * The function sets the parameters of <br>
 * the allpassfilter class
 * @return a pointer to the newly created stp_allpassfilter object <br>
 */

mf_allpass *mf_allpass_new();

/**
 * @related mf_allpass
 * @brief Frees a allpassfilter object<br>
 * @param x My allpassfilter object <br>
 * The function frees the allocated memory<br>
 * of a allpassfilter object
 */

void mf_allpass_free(mf_allpass *x);

/**
 * @related mf_allpass
 * @brief Sets the feedback & feedforward gain parameter according to "Schröder: Natural Sounding Reverberation" http://www.aes.org/e-lib/browse.cfm?elib=849 <br>
 * @param x My allpassfilter object <br>
 * @param gain The feedback gain value <br>
 * @param delay The delay value <br>
 * @param t60 The desired reverberation time<br>
 * @param fs The current sample rate <br>
 * The function sets the feedback gain parameter of <br>
 * the allpassfilter class
 */

void mf_allpass_setDelay(mf_allpass *x, int delay);

/**
 * @related mf_allpass
 * @brief Performs a allpassfilter structure in realtime <br>
 * @param x My allpassfilter object <br>
 * @param delayout The value of the buffer at given sample <br>
 * @param buffer The buffer which holds delayed samples <br>
 * @param counter The index of the current sample <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function stp_gain_perform performs the gain adjustment of <br>
 * the incoming signal and copies the result to the output vector <br>
 */

void mf_allpass_perform(mf_allpass *x, float *in, float *out, int vectorSize);

/**
 * @related mf_allpass
 * @brief Clears the buffer of the allpassfilter<br>
 * @param x My allpassfilter object <br>
 * @param buffer The buffer which holds delayed samples <br>
 * The function sets all values of the buffer to zero <br>
 */


void mf_allpass_clearBuffer(mf_allpass *x);

#endif /* mf_allpass_h */
