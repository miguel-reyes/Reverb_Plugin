/**
 * @file mf_reverb.h
 * @author Marquis Fields, Miguel Reyes Botello & Malte Schneider <br>
 * Audiocommunication Group, Technical University Berlin <br>
 * A simple combfilter <br>
 * <br>
 * @brief Audio Object for generating a multi tap delay <br>
 * <br>
 * mf_comb allows for generating a multi tap delay of desired length<br>
 * of any incoming audio signal. <br>
 * <br>
 */

#ifndef mf_comb_h
#define mf_comb_h
#include <stdio.h>
#include <stdlib.h>


/**
 * @struct mf_comb
 * @brief A structure for a comb filter <br>
 * @var mf::delay The parameter value for adjusting the <br>
 * delay in Samples of the multi tap delay
 * @var mf::counter A value for indexing the current sample <br>
 * @var mf::gain The parameter value for the recursive gain <br>
 * @var mf::buffer An array to store the delayed samples <br>
 */

typedef struct mf_comb
{
    int delay;  /**< parameter for adjusting the delay of the multi tap delay */
    int counter;    /**< parameter for indexing the current sample */
    float gain;     /**< parameter for adjusting the level of the recursive gain */
    float buffer[2000]; /**< An array to store the delayed samples */

} mf_comb;

/**
 * @related mf_comb
 * @brief Creates a new combfilter object<br>
 * The function sets the parameters of <br>
 * the combfilter class
 * @return a pointer to the newly created stp_combfilter object <br>
 */

mf_comb *mf_comb_new();

/**
 * @related mf_comb
 * @brief Frees a combfilter object<br>
 * @param x My combfilter object <br>
 * The function frees the allocated memory<br>
 * of a combfilter object
 */

void mf_comb_free(mf_comb *x);

/**
* @related mf_comb
* @brief Sets the delay parameter <br>
* @param x My combfilter object <br>
* @param delay The delay value <br>
* The function sets the delay parameter of <br>
* the combfilter class
*/

void mf_comb_setDelay(mf_comb *x, int delay);

/**
 * @related mf_comb
 * @brief Sets the feedback gain parameter according to "Schröder: Natural Sounding Reverberation" http://www.aes.org/e-lib/browse.cfm?elib=849 <br>
 * @param x My combfilter object <br>
 * @param gain The feedback gain value <br>
 * @param delay The delay value <br>
 * @param t60 The desired reverberation time<br>
 * @param fs The current sample rate <br>
 * The function sets the feedback gain parameter of <br>
 * the combfilter class
 */

void mf_comb_setGain(mf_comb *x, float t60, float fs);

/**
 * @related mf_comb
 * @brief Performs a combfilter structure in realtime <br>
 * @param x My combfilter object <br>
 * @param delayout The value of the buffer at given sample <br>
 * @param buffer The buffer which holds delayed samples <br>
 * @param counter The index of the current sample <br>
 * @param in The input vector <br>
 * @param out The output vector <br>
 * @param vectorSize The vectorSize <br>
 * The function stp_gain_perform performs the gain adjustment of <br>
 * the incoming signal and copies the result to the output vector <br>
 */

void mf_comb_perform(mf_comb *x, float *in, float *out, int vectorSize);


/**
 * @related mf_comb
 * @brief Clears the buffer of the combfilter<br>
 * @param x My combfilter object <br>
 * @param buffer The buffer which holds delayed samples <br>
 * The function sets all values of the buffer to zero <br>
 */

void mf_comb_clearBuffer(mf_comb *x);


#endif /* mf_comb_h */
