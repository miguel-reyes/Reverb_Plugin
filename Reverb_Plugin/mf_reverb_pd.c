

#include "m_pd.h"
#include "mf_allpass.h"
#include "mf_comb.h"
#include <math.h>
#include <stdbool.h>

static t_class *mf_reverb_tilde_class;

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
    
    for (int i = 0; i<n; i++)
    {
        buffer1[i] = (comb_out1[i] + comb_out2[i] + comb_out3[i] + comb_out4[i])/4;
        buffer2[i] = buffer1[i];
    }

    for( int i = 0; i<20; i++)
    {
        if (i % 2 == 0) mf_allpass_perform(x->allpass[i], buffer1, buffer1, n);
        else mf_allpass_perform(x->allpass[i], buffer2, buffer2, n);
    }
    

    for(int i = 0; i<n; i++)
    {
        outl[i] = x->level * (in[i] + x->wetLevel * buffer1[i]) ;
        outr[i] = x->level * (in[i] + x->wetLevel * buffer2[i]);
    }
    
    /* return a pointer to the dataspace for the next dsp-object */
    return (w+6);
}

void mf_reverb_tilde_dsp(mf_reverb_tilde *x, t_signal **sp)
{
    dsp_add(mf_reverb_tilde_perform, 5, x, sp[0]->s_vec, sp[1]->s_vec, sp[2]->s_vec , sp[0]->s_n);
}

void mf_reverb_tilde_free(mf_reverb_tilde *x)
{
    for (int i = 0; i < 4; i++)
    {
        mf_comb_free(x->comb[i]);
    }
    
    for (int i = 0; i < 12; i++) // probier hier mal andere i's
    {
        mf_allpass_free(x->allpass[i]);
    }
    
    outlet_free(x->x_outl);
    outlet_free(x->x_outr);
    
}

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
    
    // probier hier mal andere delays
    int dly_allpass[40] = {262,171,355,290,748,614,739,251,162,592,313,790,502,616,340,85,291,681,450,52,336,350,736,755,350,751,485,380,615,752,710,309,403,399,163,183,330,597,73,226};
    
    //int dly_allpass[40] = { 155,53,88,16,49,13,49,181,63,116,190,67,123,10,108,33,198,166,144,118,160,72,85,140,49,193,15,141,100,91,66,122,28,92,3,61,107,15,141,42};


    for(int i = 0; i < 40; i++)
    {
        x->allpass[i] = mf_allpass_new();
        mf_allpass_setDelay(x-> allpass[i], dly_allpass[i]);
        mf_allpass_clearBuffer(x->allpass[i]);
    }
    
    return (void *)x;
}

void mf_reverb_tilde_wet(mf_reverb_tilde* x, float wet)
{
    x->wetLevel = wet/100;
}

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
