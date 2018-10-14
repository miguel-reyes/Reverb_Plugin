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



#include "m_pd.h"
#include "mf_allpass.h"
#include "mf_comb.h"
#include <math.h>
#include <stdbool.h>

static t_class *mf_reverb_tilde_class;


/**
 * @struct mf_reverb_tilde
 * @brief The Pure Data struct of the mf_reverb~ object. <br>
 * @var mf_reverb_tilde::x_obj Necessary for every signal object in Pure Data <br>
 * @var mf_reverb_tilde::f Also necessary for signal objects, float dummy dataspace <br>
 * for converting a float to signal if no signal is connected (CLASS_MAINSIGNALIN) <br>
 * @var mf_reverb_tilde::mf_allpass The comb filter object for the processing <br>
 * @var mf_reverb_tilde::mf_comb The comb filter object for the processing <br>
 * @var mf_reverb_tilde::wetlevel The value of how wet/dry the mix is <br>
 * @var mf_reverb_tilde::off The boolean object for the resetting of the output <br>
 * @var mf_reverb_tilde::x_outl A signal outlet for the processed left signal <br>
 * @var mf_reverb_tilde::x_outr A signal outlet for the processed right signal
 */

typedef struct mf_reverb_tilde
{
    t_object  x_obj;
    t_sample f;
    mf_allpass *allpass[40];
    mf_comb *comb[4];
    float t60;
    t_sample level;
    float wetLevel;
    bool off;
    t_outlet *x_outl;
    t_outlet *x_outr;

    
} mf_reverb_tilde;


/**
 * @related mf_reverb_tilde
 * @brief Calculates the output vector including reverb effect<br>
 * @param w A pointer to the object, input and output vectors.<br>
 * @var mf_reverb_tilde_perform::comb_out1[n] Output of comb filter 1 <br>
 * @var mf_reverb_tilde_perform::comb_out2[n] Output of comb filter 2 <br>
 * @var mf_reverb_tilde_perform::comb_out3[n] Output of comb filter 3 <br>
 * @var mf_reverb_tilde_perform::comb_out4[n] Output of comb filter 4 <br>
 * @var mf_reverb_tilde_perform::buffer1[n] Output buffer 1 <br>
 * @var mf_reverb_tilde_perform::buffer2[n] Output buffer 2 <br>
 * @var mf_reverb_tilde_perform::off The boolean object for the resetting of the output <br>
 * @var mf_reverb_tilde::x_outl A signal outlet for the processed left signal <br>
 * @var mf_reverb_tilde::x_outr A signal outlet for the processed right signal
 * @param outl The output vector for the left channel <br>
 * @param outr The output vector for the right channel <br>
 * @param vectorSize The vectorSize <br>
 * The function stp_gain_perform performs the gain adjustment of <br>
 * the incoming signal and copies the result to the output vector <br>
 */

t_int *mf_reverb_tilde_perform(t_int *w)
{
    
    mf_reverb_tilde *x = (mf_reverb_tilde *)(w[1]);
    t_sample  *in = (t_sample *)(w[2]);
    t_sample  *outl =  (t_sample *)(w[3]);
    t_sample  *outr =  (t_sample *)(w[4]);
    int n =  (int)(w[5]);
    
    
        
    t_sample comb_out1[n];
    t_sample comb_out2[n];
    t_sample comb_out3[n];
    t_sample comb_out4[n];
    t_sample buffer1[n];
    t_sample buffer2[n];
    
    
    for (int i = 0; i < n; i++)
    {
        comb_out1[i] = 0;
        comb_out2[i] = 0;
        comb_out3[i] = 0;
        comb_out4[i] = 0;
        buffer1[i] = 0;
        buffer2[i] = 0;
    }
    
    mf_comb_perform(x->comb[0], in, comb_out1, n);
    mf_comb_perform(x->comb[1], in, comb_out2, n);
    mf_comb_perform(x->comb[2], in, comb_out3, n);
    mf_comb_perform(x->comb[3], in, comb_out4, n);
    
    /* Assigns the values of the summed comb-filtered signals to buffer1 and buffer2 */
    for (int i = 0; i<n; i++)
    {
        buffer1[i] = (comb_out1[i] + comb_out2[i] + comb_out3[i] + comb_out4[i])/4;
        buffer2[i] = buffer1[i];
    }

    /* separates the allpass-filtered signals to the buffer1 and buffer2  */
    for( int i = 0; i<20; i++)
    {
        if (i % 2 == 0) mf_allpass_perform(x->allpass[i], buffer1, buffer1, n);
        else mf_allpass_perform(x->allpass[i], buffer2, buffer2, n);
    }
    
    /* The original signal is mixed with the processed signal */
    for(int i = 0; i<n; i++)
    {
        outl[i] = x->level * (in[i] + x->wetLevel * buffer1[i]) ;
        outr[i] = x->level * (in[i] + x->wetLevel * buffer2[i]);
    }
    
    /* return a pointer to the dataspace for the next dsp-object */
    return (w+6);
}

/**
 * @related mf_reverb_tilde
 * @brief Adds mf_reverb_tilde_perform to the signal chain. <br>
 * @param x A pointer the stp_gain_tilde object <br>
 * @param sp A pointer the input and output vectors <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void mf_reverb_tilde_dsp(mf_reverb_tilde *x, t_signal **sp)
{
    dsp_add(mf_reverb_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec , sp[0]->s_n);
}

/**
 * @related mf_reverb_tilde
 * @brief Frees our object<br>
 * @param x A pointer the mf_reverb_tilde object <br>
 * The function frees the allocated memory<br>
 * of a reverb object
 */
void mf_reverb_tilde_free(mf_reverb_tilde *x)
{
    for (int i = 0; i < 4; i++)
    {
        mf_comb_free(x->comb[i]);
    }
    
    for (int i = 0; i < 40; i++) 
    {
        mf_allpass_free(x->allpass[i]);
    }
    
    outlet_free(x->x_outl);
    outlet_free(x->x_outr);
    
}

/**
 * @related mf_reverb_tilde
 * @brief Panic button to mute output<br>
 * @param x A pointer the mf_reverb_tilde object <br>
 * The function mutes output<br>
 * of the reverb object
 */
void mf_reverb_tilde_panic(mf_reverb_tilde *x)
{
    if (x->off == false)
    {
        x->level = 0;
        x->off = true;
    }
    else
    {
        x->level = 1;
        x->off = false;
    }
}

/**
 * @related mf_reverb_tilde
 * @brief Creates a new mf_reverb_tilde object.<br>
 * @param f sets the reverberation time in seconds. <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void *mf_reverb_tilde_new(t_floatarg f)
{
    mf_reverb_tilde *x = (mf_reverb_tilde *)pd_new(mf_reverb_tilde_class);
    //The main inlet is created automatically
    x->x_outl = outlet_new(&x->x_obj, &s_signal);
    x->x_outr = outlet_new(&x->x_obj, &s_signal);
    x->t60 = f;
    x->level = 1;
    x->off = false;

    for (int i = 0; i<4; i++)
    {
        x->comb[i] = mf_comb_new();
        mf_comb_setDelay(x-> comb[i], floor((.03 + i*.005) * sys_getsr()));
        mf_comb_setGain(x-> comb[i], x->t60, sys_getsr());
        mf_comb_clearBuffer(x->comb[i]);
    }
    
    
    int dly_allpass[40] = {262,171,355,290,244,327,487,251,162,592,313,432,502,616,340,85,291,119,450,52,336,350,326,159,350,482,485,380,468,222,74,309,403,399,163,183,330,321,73,226};
    
  

    for(int i = 0; i < 40; i++)
    {
        x->allpass[i] = mf_allpass_new();
        mf_allpass_setDelay(x-> allpass[i], dly_allpass[i]);
        mf_allpass_clearBuffer(x->allpass[i]);
    }
    
    return (void *)x;
}

/**
 * @related mf_reverb_tilde
 * @brief Sets the wet level of the reverb<br>
 * @param x A pointer the mf_reverb_tilde object <br>
 * @param wet Value from the slider to set wet level <br>
 * The function sets the wet level<br>
 * of the reverb object
 */
void mf_reverb_tilde_wet(mf_reverb_tilde* x, float wet)
{
    x->wetLevel = wet/200;
}

/**
 * @related mf_reverb_tilde
 * @brief Setup of mf_reverb_tilde <br>
 * For more information please refer to the <a href = "https://github.com/pure-data/externals-howto" > Pure Data Docs </a> <br>
 */
void mf_reverb_tilde_setup(void)
{
      mf_reverb_tilde_class = class_new(gensym("mf_reverb~"),
            (t_newmethod)mf_reverb_tilde_new,
            (t_method)mf_reverb_tilde_free,
        sizeof(mf_reverb_tilde),
            CLASS_DEFAULT,
            A_DEFFLOAT, 0);

    class_addmethod(mf_reverb_tilde_class, (t_method)mf_reverb_tilde_dsp, gensym("dsp"), 0);
    class_addmethod(mf_reverb_tilde_class, (t_method)mf_reverb_tilde_wet, gensym("wet"), A_DEFFLOAT,0);
    class_addbang(mf_reverb_tilde_class, mf_reverb_tilde_panic);
    


      CLASS_MAINSIGNALIN(mf_reverb_tilde_class, mf_reverb_tilde, f);
}
